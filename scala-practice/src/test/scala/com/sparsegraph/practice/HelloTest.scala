import com.sparsegraph.practice._
import org.scalatest.FunSuite

class HelloTest extends FunSuite {
  test("Check graph size") {
    assert(true)
    assert(Graph(List(Vertex(1))).size == 1)
  }
}
