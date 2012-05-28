import com.sparsegraph.sandbox

import org.scalatest.FunSuite
import scala.collection.JavaConverters._
import scala.collection.JavaConversions._

import com.orientechnologies.orient.core.db.graph._

class OrientChainBench extends FunSuite {
  test( "OrientPersistencyCheck" ) {
    assert(true)
    val dbPath: String = "/tmp/ori-test"
    val uri: String = "local:" + dbPath

    // with save()
    {
      val db: OGraphDatabase = new OGraphDatabase(uri)
      assert(! db.exists())
      assert(db.isClosed())

      db.create()
      assert(db.exists())
      assert(! db.isClosed())

      val rootNode = db.createVertex().field("id", 0).save()
      val newNode = db.createVertex().field("id", 1).save()
      newNode.field("brand", "Hyundai").field("model", "Coupe")
      val edge = db.createEdge( rootNode, newNode ).save()
      db.setRoot("testGraph", rootNode)

      db.close()
      assert(db.exists())
      assert(db.isClosed())
    }

    // persistence check
    {
      val db: OGraphDatabase = new OGraphDatabase(uri)
      assert(db.exists())
      assert(db.isClosed())

      db.open("admin", "admin")
      assert(! db.isClosed())

      val rootNode = db.getRoot("testGraph")
      assert(rootNode.field("id") == 0)

      for (edge <- db.getOutEdges(rootNode)) {
        val inNode  = db.getInVertex(edge)
        val outNode = db.getOutVertex(edge)

        assert(inNode.field("id")  == 1)
        assert(inNode.field("brand") == "Hyundai")
        assert(outNode.field("id") == 0)
      }

      db.delete() // deprecated
      assert(! db.exists())
      assert(db.isClosed())
    }

    // without using save()
    {
      val db: OGraphDatabase = new OGraphDatabase(uri)
      assert(! db.exists())
      assert(db.isClosed())

      db.create()
      assert(db.exists())
      assert(! db.isClosed())

      val rootNode = db.createVertex().field("id", 0)
      val newNode = db.createVertex().field("id", 1)
      newNode.field("brand", "Hyundai").field("model", "Coupe")
      val edge = db.createEdge( rootNode, newNode )
      db.setRoot("testGraph", rootNode)

      db.close()
      assert(db.exists())
      assert(db.isClosed())
    }

    // persistence check
    {
      val db: OGraphDatabase = new OGraphDatabase(uri)
      assert(db.exists())
      assert(db.isClosed())

      db.open("admin", "admin")
      assert(! db.isClosed())

      val rootNode = db.getRoot("testGraph")
      assert(rootNode.field("id") == 0)

      for (edge <- db.getOutEdges(rootNode)) {
        val inNode  = db.getInVertex(edge)
        val outNode = db.getOutVertex(edge)

        assert(inNode.field("id")  == 1)
        assert(inNode.field("brand") == "Hyundai")
        assert(outNode.field("id") == 0)
      }

      db.delete() // deprecated
      assert(! db.exists())
      assert(db.isClosed())
    }
  }
}
