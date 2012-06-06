package com.sparsegraph.sandbox

import scala.compat.Platform
import org.apache.commons.math3.random._
import org.apache.commons.math3.special._
import scala.collection.mutable.HashMap

trait SI {
  val randomData = new RandomDataImpl()
  val trial = 30
  val beta  = 0.1

  var gr = new HashMap[Int, HashMap[Int, Double]]
  var st = new HashMap[Int, String]

  def update(nodeId: Int, beforeStatus: String) = {
    var afterStatus = "S"

    beforeStatus match {
      case "S" => {
        val change = getNeighbors(nodeId).find { link =>
          val (src, weight) = link

          (st.get(src).getOrElse("S") == "I" && diceWithProb(beta))
        }
        if (change.isDefined)
          afterStatus = "I"
      }
      case "I" => afterStatus = "I"
    }

    (nodeId, afterStatus)
  }

  def diceWithProb(prob: Double): Boolean = {
    (randomData.nextLong(0, 100) < prob * 100)
  }

  def getNeighbors(nodeId: Int) = {
    gr(nodeId) // warning
  }

  def run() {
    setup()

    for (iteration <- 1 until trial) {
      var next_st = new HashMap[Int, String]
      var freq = new HashMap[String, Int]

      gr foreach { entry =>
        val (nodeId, neighbors) = entry

        st.get(nodeId) match {
          case Some(infection) => {
            val s = update(nodeId, infection)
            next_st += s
            freq(s._2) = freq.get(s._2).getOrElse(0) + 1
          }
          case None =>
        }
      }

      st = next_st
      printf("Iter: %02d, " + freq + "\n", iteration)
      if (freq.size == 1) return
    }
  }

  def setup() {
    gr += (0 -> new HashMap[Int, Double])
    gr += (1 -> new HashMap[Int, Double])
    gr += (2 -> new HashMap[Int, Double])
    gr += (3 -> new HashMap[Int, Double])
    gr += (4 -> new HashMap[Int, Double])

    /*
     *  0(S) <- 1(S) <- 2(S) <- 3(S) <- 4(I)
     *    ^      ^        |      |
     *    |      |        |      |
     *    x------x--------x      |
     *           x---------------x
     */

    /* node ids & weights linked from argument node */
    gr(0) += (1 -> 0.5)
    gr(0) += (2 -> 0.5)
    gr(1) += (2 -> 0.5)
    gr(1) += (3 -> 0.5)
    gr(2) += (3 -> 0.5)
    gr(3) += (4 -> 0.5)

    st += (0 -> "S")
    st += (1 -> "S")
    st += (2 -> "S")
    st += (3 -> "S")
    st += (4 -> "I")
  }
}
