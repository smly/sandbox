package com.sparsegraph.sandbox

import scala.compat.Platform
import scala.collection.JavaConverters._
import scala.collection.JavaConversions._

import org.apache.commons.math3.stat.descriptive._
import com.orientechnologies.orient.core.db.graph._
import org.neo4j.graphdb.{Node,DynamicRelationshipType}
import org.neo4j.kernel.EmbeddedGraphDatabase

import java.io.File

object Main {
  def main(args: Array[String]) {
    if (args.size < 1) {
      println("please specify target className")
      return
    }
    args(0) match {
      case "Chain" => new Chain { }.run()
      case "SI" => new SI { }.run()
      case _ => println("No such className")
    }
  }
}
