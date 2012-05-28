package com.sparsegraph.sandbox

import scala.compat.Platform
import scala.collection.JavaConverters._
import scala.collection.JavaConversions._
import org.apache.commons.math3.stat.descriptive._

import com.orientechnologies.orient.core.db.graph._
import com.orientechnologies.orient.core.db.document._
import com.orientechnologies.orient.core.intent.OIntentMassiveInsert
import com.orientechnologies.orient.core.config.OGlobalConfiguration

import org.neo4j.graphdb.{Node,DynamicRelationshipType}
import org.neo4j.kernel.EmbeddedGraphDatabase

import java.io.File

trait Chain {
  def run() {
    for (i <- 1 to 5) {
      orientChainBench( i*10*1000 )
      orientWithIntentChainBench( i*10*1000, 1, true,  "orient (Intent)" )
      orientWithIntentChainBench( i*10*1000, 1, false, "orient (Intent+TxOff)" )
      orientWithIntentChainBench( i*10*1000, 10000, false, "orient (Intent+TxOff+Stack10k)" )
      neoChainBench( i*10*1000 )
    }
  }

  def toCSV(label: String, sz: Int, res: List[(Long, Long)]) {
    for (pair <- res) {
      printf("> %s,%d,%.3f,%.3f\n", label, sz, pair._1 / 10.0e+5, pair._2 / 1024.0 / 1024.0)
    }
  }

  def summarize(res: List[(Long, Long)]) {
    var stats = new SummaryStatistics
    var i = 1; res foreach { pair =>
      val elapsedNanoTime = pair._1
      val storedSize = pair._2

      printf("Trial # %d: Elapsed time = %.3f msec, %.3f MB\n",
             i, elapsedNanoTime / 10.0e+5,
             storedSize / 1024.0 / 1024.0)

      i += 1
      stats.addValue(elapsedNanoTime / 10.0e+5)
    }

    printf("Mean Elapsed time       = %.3f msec\n", stats.getMean)
    printf("Standard Deviation      = %.3f msec\n", stats.getStandardDeviation)
  }

  def orientWithIntentChainBench(graphSize: Int, saveInterval: Int, tx: Boolean, label: String) {
    print("for OrientDB MII (Constructing line graph): ")
    val summary = new OrientBenchmark {
      def run() {
        print(".")
        if (!tx) OGlobalConfiguration.TX_USE_LOG.setValue(false)

        var db: OGraphDatabase = new OGraphDatabase(uri)
        if (!db.exists())  db.create()
        if (db.isClosed()) db.open("admin", "admin")

        db.declareIntent(new OIntentMassiveInsert())

        val rootNode = db.createVertex().field("id", 0).save()
        db.setRoot("graph", rootNode)

        var currentNode = rootNode
        for (i <- 1 to graphSize) {
          val newNode = db.createVertex().field("id", i).save()
          val edge = db.createEdge( currentNode, newNode )
          if (i % saveInterval == 0) edge.save()

          currentNode = newNode
        }

        db.declareIntent(null)

        db.close()
      }
    }.runBenchmark(10); println()
    summarize(summary)
    toCSV(label, graphSize, summary)
  }

  def orientChainBench(graphSize: Int) {
    print("for OrientDB (Constructing line graph): ")
    val summary = new OrientBenchmark {
      def run() {
        print(".")
        var db: OGraphDatabase = new OGraphDatabase(uri)
        if (!db.exists())  db.create()
        if (db.isClosed()) db.open("admin", "admin")

        val rootNode = db.createVertex().field("id", 0).save()
        var currentNode = rootNode
        for (i <- 1 to graphSize) {
          val newNode = db.createVertex().field("id", i).save()
          val edge = db.createEdge( currentNode, newNode )
          edge.save()
          currentNode = newNode
        }

        db.close()
      }
    }.runBenchmark(10); println()
    summarize(summary)
    toCSV("orient", graphSize, summary)
  }

  def neoChainBench(graphSize: Int) {
    print("for Neo4j (Conntructing line graph): ")
   val summary = new NeoBenchmark {

      def run() {
        print(".")
        val db = new EmbeddedGraphDatabase( dbPath )
        val tx = db.beginTx

        var rootNode = db.createNode
        rootNode.setProperty("id", 0)
        var currentNode = rootNode
        var rel = DynamicRelationshipType.withName("rel")
        for (i <- 1 to graphSize) {
          val newNode = db.createNode
          newNode.setProperty("id", i)
          currentNode.createRelationshipTo( newNode, rel )
        }

        tx.success
        tx.finish
        db.shutdown
      }
    }.runBenchmark(10); println()
    summarize(summary)
    toCSV("neo4j", graphSize, summary)
  }
}
