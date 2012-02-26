#!/usr/bin/ruby

class AssociationRules
  def initialize
    @triples = Hash.new(0)
  end

  def add(filename)
    cont = open(filename).read
    baskets = cont.split("-1\n").collect {|gr|
      gr.chop.split("\n").map {|i| i.to_i }
    }
    baskets.each {|basket|
      1.upto(basket.length).each {|support|
        basket.combination(support).each {|comb|
          @triples[comb.sort] += 1
        }
      }
    }
  end

  def frequent_itemsets(threshold)
    @triples.reject {|k, v| v < threshold }
  end
end

ar = AssociationRules.new
ar.add("sample_input.txt")
p ar.frequent_itemsets(3)
