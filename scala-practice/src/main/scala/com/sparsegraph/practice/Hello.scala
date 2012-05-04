package com.sparsegraph.practice

object Hello {
  def main(args: Array[String]) {
    val vs = List(Vertex(1), Vertex(2))
    val g = Graph(vs)

    println("graph size: " + g.size.toString)
  }
}
