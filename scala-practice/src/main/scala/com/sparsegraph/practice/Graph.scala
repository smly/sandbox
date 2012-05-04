package com.sparsegraph.practice

object Vertex {
  def apply[A](data:A) = new Vertex[A](data)
}
class Vertex[A](val data:A) {
  var edges:List[Edge[A]] = List()
}

object Edge {
  def apply[A](
    w:Double,
    src:Vertex[A],
    dst:Vertex[A]) = new Edge[A](w, src, dst)
}
class Edge[A](
  var weight:Double,
  val src:Vertex[A],
  val dst:Vertex[A]) {

}

object Graph {
  def apply[A](vs:List[Vertex[A]]) = new Graph[A](vs)
}
class Graph[A](val vs:List[Vertex[A]]) {
  private[practice] var vertices = vs

  def size = vertices.size
}
