#!/usr/bin/env ruby
# encoding: utf-8
# using ruby 1.9.2p180

# demo of DGIM method

# ref:
# Mayur Datar, Aristides Gionis, Piotr Indyk, and Rajeev Motwani
# "Maintaining stream statistics over sliding windows",
# SIAM J. Comput. 31, pp. 1794-1813 (2002)

module DataMiningRindoku
  class DemoStream
    def initialize(randomness = 5)
      @randomness = randomness
      @stream     = get_stream(@randomness)
    end

    def get_stream(randomness)
      return Fiber.new do
        loop do
          Fiber.yield(rand(randomness) == 0 ? 1 : 0)
        end
      end
    end

    def get
      return @stream.resume
    end
  end

  class Bucketized
    class Bucket
      def initialize(endtime, num)
        @endtime = endtime
        @num     = num
      end

      def get_num
        @num
      end

      def get_endtime
        @endtime
      end

      def to_s
        "{#{@num.to_s}.#{@endtime}}"
      end
    end

    # Bucketized representation
    def initialize(init)
      @orig        = init
      @window_size = init.size
      @head        = @window_size
      @buckets     = bucketize(init)
      @mutex       = Mutex.new

      # if leftmost bucket's endtime == head; then remote it
      @mutex.synchronize do
        @buckets.pop if check_leftmost
      end
    end

    def check_leftmost
      @buckets.size > 0 and @head - @buckets.last.get_endtime >= @window_size
    end

    def count_last(k)
      total = 0
      @mutex.synchronize do
        prev  = @head
        @buckets.each do |bucket|
          # get exact answer using leftmost bucket within the window
          if bucket.get_endtime <= @head - k
            break if prev - 1 == @head - k
            begin_pos = @head - (prev - 1)
            end_pos   = @head - (@head - k + 1)
            begin_pos.upto(end_pos) do |pos|
              total += 1 if @orig[@window_size - pos - 1] == 1
            end
            break
          end

          total += bucket.get_num
          prev = bucket.get_endtime
        end
      end
      total
    end

    def input(bit)
      @head += 1
      @buckets.pop if check_leftmost

      @orig.push(bit)
      @orig.shift

      if bit == 1
        @buckets.unshift(Bucket.new(@head, 1))
        bucketsize = 1
        samesize_count = 0
        newbuf = Array.new

        while not @buckets.empty?
          bucket = @buckets.shift
          if bucket.get_num == bucketsize
            samesize_count += 1
            if samesize_count > 2
              # merge
              newbuf[-1] = Bucket.new(bucket.get_endtime, bucketsize * 2)
              buketsize = bucketsize * 2
              samesize_count = 1
            else
              newbuf << bucket
            end
          else
            bucketsize = bucketsize * 2
            samesize_count = 1
            newbuf << bucket
          end
        end
        @buckets = newbuf
      end
    end

    def bucketize(init)
      bucket_size = 1
      one_counter = 0
      buf = []

      last_pos = @window_size - 1
      last_pos.downto(0) do |current_pos|
        one_counter += 1 if init[current_pos] == 1
        if one_counter == bucket_size
          buf << Bucket.new(current_pos, bucket_size)
          one_counter = 0
          bucket_size = bucket_size * 2
        end
      end

      buf
    end

    def size
      @buckets.size
    end

    def to_s
      @buckets.empty? ? "{}" : @buckets.join(",")
    end
  end
end

module DemoRindoku
  require 'optparse'
  require 'thread'

  include DataMiningRindoku

  class << self
    def optparse
      params = {
        :window  => 20000,
        :bitprob => 10, # generate 1 w.p. 1/10
        :last    => 10000,
      }
      op = OptionParser.new do |opars|
        opars.on('-w SIZE', '--window', 'stream window size') do |v|
          params[:window] = v.to_i
        end
        opars.parse!(ARGV)
      end

      params
    end

    def run
      Signal.trap(:INT){ exit(0) }
      Signal.trap('SIGPIPE', 'IGNORE')

      params = optparse
      # generate 1 w.p. 1/10
      s = DataMiningRindoku::DemoStream.new(params[:bitprob])
      w = DataMiningRindoku::Bucketized.new(Array.new([0]*params[:window]))
      bitcount = 0

      Thread.new do
        loop do
          total_time = Time.now.usec
          count      = w.count_last(params[:last])
          total_time = Time.now.usec - total_time

          output = sprintf "% 8d: last% 5d =", bitcount, params[:last]
          output += sprintf "% 5d", count
          output += sprintf ", bucketsize =% 4d", w.size
          output += sprintf ", windowsize =% 5d", params[:window]
          output += sprintf ", utime = %.04f msec", total_time / 1000.0
          STDOUT.print output
          STDOUT.flush

          sleep 1
          STDOUT.print "\b" * output.size
        end
      end

      loop do
        bitcount += 1
        w.input( s.get )
      end
    end

  end
end

if $0 == __FILE__
  DemoRindoku.run
end
