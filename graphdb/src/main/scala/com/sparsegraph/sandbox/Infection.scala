package com.sparsegraph.sandbox

import scala.compat.Platform
import org.apache.commons.math3.random._
import org.apache.commons.math3.special._
import scala.collection.mutable.HashMap
import scala.collection.mutable.HashSet
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

/*
 *  0(S) <- 1(S) <- 2(S) <- 3(S) <- 4(I)
 *    ^      ^        |      |
 *    |      |        |      |
 *    x------x--------x      |
 *           x---------------x
 */
trait SIR extends Infection {
  val deathRate = 0.9

  override def update(node: Long, status: Char) = {
    var nextStatus = status

    status match {
      case 'S' => {
        val morph = gr.walkIncommingEdge(node).find { src =>
          ( st.get(src).getOrElse('S') == 'I' && diceWithProb(birthRate) )
        }
        if (morph.isDefined) nextStatus = 'I'
      }
      case 'I' => {
        if (diceWithProb(deathRate))
          nextStatus = 'R'
      }
      case 'R' => nextStatus = 'R'
    }

    (node -> nextStatus)
  }
}

trait SIS extends Infection {
  val deathRate = 0.9

  override def update(node: Long, status: Char) = {
    var nextStatus = status

    status match {
      case 'S' => {
        val morph = gr.walkIncommingEdge(node).find { src =>
          ( st.get(src).getOrElse('S') == 'I' && diceWithProb(birthRate) )
        }
        if (morph.isDefined) nextStatus = 'I'
      }
      case 'I' => {
        if (diceWithProb(deathRate))
          nextStatus = 'S'
      }
    }

    (node -> nextStatus)
  }
}

trait SI extends Infection {
  override def update(node: Long, status: Char) = {
    var nextStatus = status

    status match {
      case 'S' => {
        val change = gr.walkIncommingEdge(node).find { src =>
          ( st.get(src).getOrElse('S') == 'I' && diceWithProb(birthRate) )
        }
        if (change.isDefined) nextStatus = 'I'
      }
      case 'I' => nextStatus = 'I'
    }

    (node -> nextStatus)
  }
}

trait Infection {
  val randomData = new RandomDataImpl()
  val gr   = Graph()
  var st   = new HashMap[Long, Char]
  val freq = new HashMap[Char, Int]

  val trial = 30
  val birthRate  = 0.2

  def getFreq(c: Char) =
    freq.get(c).getOrElse(0)

  def setup() {
    gr.addEdges( List((1, 0), (2, 1), (3, 2), (4, 3), (2, 0), (3, 1)) )
    for ((x, y) <- gr.walkNodes) st += (x -> 'S')
    st(4) = 'I'
  }

  def diceWithProb(prob: Double): Boolean = {
    (randomData.nextLong(0, 100) <= prob * 100)
  }

  def update(node: Long, status: Char) = {
    (node -> status)
  }

  def run() {
    setup()

    for (iter <- 1 to trial) {
      for ((k,v) <- freq) freq(k) = 0
      val next_st = new HashMap[Long, Char]

      for ((node, links) <- gr.walkNodes) {
        st.get(node) match {
          case Some(currentStatus) => {
            val nextStatus = update(node, currentStatus)
            next_st += nextStatus
            freq(nextStatus._2) = freq.get(nextStatus._2).getOrElse(0) + 1
          }
          case None =>
        }
      }

      st = next_st
      for ((k,v) <- freq) if (freq(k) == gr.size) return
    }
  }
}

