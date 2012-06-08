package com.sparsegraph.sandbox

import scala.compat.Platform
import org.apache.commons.math3.random._
import org.apache.commons.math3.special._
import scala.collection.mutable.HashMap
import scala.collection.mutable.HashSet

import org.neo4j.graphdb.{Node,DynamicRelationshipType}
import org.neo4j.kernel._

import java.io.File

trait GraphTrait[V] {
  def addEdges(edges: List[(V, V)]) {
    edges foreach { edge =>
      val (src, dst) = edge
      addEdge(src, dst)
    }
  }

  def addEdge(src: V, dst: V)
  def exists(node: V): Boolean
}

object Graph {
  def apply() = new Graph[Long](List())
  def apply(vs:List[Long]) = new Graph[Long](vs)
}

class GraphDB[V] (val dbPath:String) extends GraphTrait[V] {
  val db = new EmbeddedGraphDatabase( dbPath )
  val tx = db.beginTx

  def shutdown {
    tx.success
    tx.finish
    db.shutdown
  }

  override def addEdge(src: V, dst: V) {
    
  }

  override def exists(node: V) = {
    false
  }
}

class Graph[V] (val vs:List[V]) extends GraphTrait[V] {
  val vertices = new HashMap[V, HashSet[V]]
  vs.foreach ( x => vertices += (x -> new HashSet[V] ) )

  def walkIncommingEdge(v: V) = {
    for (x <- vertices(v)) yield x
  }

  def walkNodes = {
    for (x <- vertices) yield x
  }

  override def addEdge(src: V, dst: V) {
    if (! vertices.contains(src))
      vertices += (src -> new HashSet[V])
    if (! vertices.contains(dst))
      vertices += (dst -> new HashSet[V])

    vertices.get(dst) match {
      case Some(x) => x += src
      case None    =>
    }
  }

  def getInCommingEdges(l:V) = vertices.get(l)

  def addNode(l:V) {
    if (! vertices.contains(l))
      vertices += (l -> new HashSet[V])
  }

  override def exists(node: V) =
    vertices.contains(node)

  def size = vertices.size
}
