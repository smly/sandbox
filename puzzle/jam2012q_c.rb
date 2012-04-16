# -*- coding: euc-jp -*-
def count_recycled(a, end_val)
  orig = String.new(a)
  count = 0
  loop do
    digit = a.slice!(0)
    a << digit
    break if orig == a
    next if a[0] == '0'
    count += 1 if orig < a and a.to_i <= end_val
  end

  count
end

def listing_recycled(a, end_val, pre_pairs)
  orig = String.new(a)
  pairs = []
  loop do
    digit = a.slice!(0)
    a << digit
    break if orig == a
    next if a[0] == '0'
    if orig < a and a.to_i <= end_val
      pairs << a.to_i
    end
  end

  pre_pairs[orig.to_i] = pairs
end

HASH_OBJ = "c_hash.dump"

pre_pairs = Hash.new
if not FileTest.exists?(HASH_OBJ)
  10.upto(99) {|i| listing_recycled(i.to_s, 99, pre_pairs) }
  100.upto(999) {|i| listing_recycled(i.to_s, 999, pre_pairs) }
  1000.upto(9999) {|i| listing_recycled(i.to_s, 9999, pre_pairs) }
  10000.upto(99999) {|i| listing_recycled(i.to_s, 99999, pre_pairs) }
  100000.upto(999999) {|i| listing_recycled(i.to_s, 999999, pre_pairs) }
  1000000.upto(2000000) {|i| listing_recycled(i.to_s, 2000000, pre_pairs) }

  hash_dump = Marshal.dump(pre_pairs)
  open(HASH_OBJ, 'w') do |dst|
    dst.write(hash_dump)
  end
else
  pre_pairs = Marshal.load(open(HASH_OBJ, 'r').read)
end

1.upto(readline.to_i) do |k|
  start_val, end_val = readline.split(' ').map{|v| v.to_i}
  cnt = 0
  start_val.upto(end_val) do |i|
    next if not pre_pairs.has_key?(i)
    pre_pairs[ i ].each do |v|
      cnt += 1 if v <= end_val
    end
  end
  puts "Case ##{k}: #{cnt}"
end
