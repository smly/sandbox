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

import org.apache.commons.math3.stat._
import org.apache.commons.math3.linear._

import scala.util.Random._

object MatrixFactorizationBenchmark {
  def p(m: RealMatrix) {
    val (row, col) = (m.getRowDimension(), m.getColumnDimension())
    for (i <- 0 until row) {
      for (j <- 0 until col)
        print(m.getData()(i)(j) + " ")
      println()
    }
  }

  def CalcurateLeastSquareError(
    orig: Array2DRowRealMatrix,
    solved: Array2DRowRealMatrix) {

  }

  def main(args: Array[String]) {
    var matrixData = Array(Array(1.0, 2.0, 3.0), Array(2.0, 5.0, 3.0))
    var m = new Array2DRowRealMatrix(matrixData)

    val solv = new SingularValueDecomposition(m)
    val u = solv.getU()
    val v = solv.getVT()
    val s = solv.getS()
    val resolved = u.multiply(s).multiply(v)

    println("original: ")
    p(m)
    println("SVD: ")
    p(resolved)
  }
}
