package com.sparsegraph.practice

import org.apache.commons.math3._
import org.apache.commons.math3.analysis._ 
import org.apache.commons.math3.analysis.function._ 
import org.apache.commons.math3.analysis.integration._ 
import org.apache.commons.math3.analysis.interpolation._ 
import org.apache.commons.math3.analysis.polynomials._ 
import org.apache.commons.math3.analysis.solvers._ 
import org.apache.commons.math3.complex._ 
import org.apache.commons.math3.dfp._ 
import org.apache.commons.math3.distribution._ 
//import org.apache.commons.math3.estimation._ 
import org.apache.commons.math3.exception._ 
import org.apache.commons.math3.exception.util._ 
import org.apache.commons.math3.filter._ 
import org.apache.commons.math3.fraction._ 
import org.apache.commons.math3.genetics._ 
import org.apache.commons.math3.geometry._ 
import org.apache.commons.math3.geometry.euclidean.oned._ 
import org.apache.commons.math3.geometry.euclidean.threed._ 
import org.apache.commons.math3.geometry.euclidean.twod._ 
import org.apache.commons.math3.geometry.partitioning._ 
import org.apache.commons.math3.geometry.partitioning.utilities._ 
import org.apache.commons.math3.ode._ 
import org.apache.commons.math3.ode.events._ 
import org.apache.commons.math3.ode.nonstiff._
import org.apache.commons.math3.ode.sampling._
import org.apache.commons.math3.optimization._
import org.apache.commons.math3.optimization.direct._
import org.apache.commons.math3.optimization.fitting._
import org.apache.commons.math3.optimization.general._
import org.apache.commons.math3.optimization.linear._
import org.apache.commons.math3.optimization.univariate._
import org.apache.commons.math3.random._
import org.apache.commons.math3.special._
import org.apache.commons.math3.stat.clustering._
import org.apache.commons.math3.stat.correlation._
import org.apache.commons.math3.stat.descriptive._
import org.apache.commons.math3.stat.descriptive.moment._
import org.apache.commons.math3.stat.descriptive.rank._
import org.apache.commons.math3.stat.descriptive.summary._
import org.apache.commons.math3.stat.inference._
import org.apache.commons.math3.stat.ranking._
import org.apache.commons.math3.stat.regression._
import org.apache.commons.math3.transform._
import org.apache.commons.math3.util._

import scala.collection.mutable.HashMap

object Main {
  val randomData = new RandomDataImpl()
  var edges = new HashMap[Int, Int]

  def drowProb(prob: Long): Boolean = {
    (randomData.nextLong(0, 100) < prob)
  }

  def read(in: java.io.InputStream, f:(String) => Unit) = {
    try {
      val reader = new java.io.BufferedReader( new java.io.InputStreamReader(in) )
      var line: String = null

      while ( { line = reader.readLine; line != null} ) f(line)
    } finally {
      in.close
    }
  }

  def main(args: Array[String]) {
    val path = "./src/main/resources/input_gr"
    val in = new java.io.FileInputStream(path)

    read(in, (l:String) => {
      val elems = l.split("\t")
      if (elems.size ==2) edges += (elems(0).toInt -> elems(1).toInt)
    })

    /*
    val trial = 10000
    for (probInfect <- 0 until 30) {
      var c = 0; for (i <- 0 until 100 * trial)
        if (drowProb(probInfect)) c += 1

      printf("Prob: %.3f / Gen: %d\n", c * 1.0 / trial, probInfect)
    }
    */
  }
}
