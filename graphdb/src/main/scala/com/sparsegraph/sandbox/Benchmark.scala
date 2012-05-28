package com.sparsegraph.sandbox

import scala.compat.Platform
import com.orientechnologies.orient.core.id.ORecordId
import com.orientechnologies.orient.core.sql.query.OSQLSynchQuery
import com.orientechnologies.orient.core.db.document._
import com.orientechnologies.orient.core.db.graph._
import com.orientechnologies.orient.core.record.impl.ODocument
import org.neo4j.kernel.EmbeddedGraphDatabase
import org.neo4j.kernel.EmbeddedGraphDatabase
import org.neo4j.graphdb.factory.GraphDatabaseFactory
import org.neo4j.graphdb.GraphDatabaseService

import java.io.File

trait NeoBenchmark extends Benchmark {
  val dbPath: String = "/tmp/neo1"
  var uri: String = dbPath

  override def setUp() {  }

  override def tearDown() {
    deleteFileOrDirectory(new File(dbPath))
  }
}

trait OrientBenchmark extends Benchmark {
  val dbPath: String = "/tmp/ori1"
  var uri: String = "local:" + dbPath
  val db: OGraphDatabase = new OGraphDatabase(uri)

  override def setUp() {
  }

  override def tearDown() {
    val db: OGraphDatabase = new OGraphDatabase(uri)
    if (db.exists()) {
      if (db.isClosed()) db.open("admin", "admin")
      db.delete() // deprecated
    }

    deleteFileOrDirectory(new File(dbPath))
  }
}

trait Benchmark {
  val dbPath: String

  def run()

  var multiplier = 1

  def runBenchmark(noTimes: Int): List[(Long,Long)] =
    for (i <- List.range(1, noTimes + 1)) yield {
      setUp
      var storedSize: Long = 0
      val startTime = System.nanoTime
      var i = 0; while (i < multiplier) {
        run()
        i += 1
      }
      val stopTime = System.nanoTime
      storedSize = getFileOrDirectorySize(new File(dbPath))
      tearDown
      Platform.collectGarbage

      (stopTime - startTime, storedSize)
    }

  def setUp() { }

  def tearDown() { }

  def getFileOrDirectorySize(file: File): Long = {
    var res: Long = 0
    if ( file.exists() ) {
      if ( file.isDirectory() ) {
        file.listFiles() foreach { child =>
          res += getFileOrDirectorySize( child )
        }
      }
      else {
        res = file.length()
      }
    }

    res
  }

  def deleteFileOrDirectory(file: File) {
    if ( file.exists() ) {
        if ( file.isDirectory() ) {
          file.listFiles() foreach { child =>
            deleteFileOrDirectory( child )
          }
        }

        file.delete();
      }
  }
}
