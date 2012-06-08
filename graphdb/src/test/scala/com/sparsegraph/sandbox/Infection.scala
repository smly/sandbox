import com.sparsegraph.sandbox._
import org.apache.commons.math3.random._
import org.apache.commons.math3.special._
import org.scalatest.FunSuite
import scala.collection.mutable.HashMap

class InfectionSIRModelTest extends FunSuite {
  test("INfection SIR-model deathRate=1") {
    val sirModel = new SIR {
      override val birthRate = 0.0
      override val deathRate = 1.0
      override val trial     = 1
    }
    sirModel.run()
    assert(sirModel.getFreq('I') == 0)
    assert(sirModel.getFreq('S') == 4)
    assert(sirModel.getFreq('R') == 1)
  }
}

class InfectionSISModelTest extends FunSuite {
  test("Infection SIS-model deathRate=1") {
    val sisModel = new SIS {
      override val birthRate = 0.0
      override val deathRate = 1.0
      override val trial     = 1
    }
    sisModel.run()
    assert(sisModel.getFreq('I') == 0)
    assert(sisModel.getFreq('S') == 5)
  }
}

class InfectionSIModelTest extends FunSuite {
  test("Infection SI-model birthRate=0") {
    val siModel = new SI {
      override val birthRate = 0.0
    }
    siModel.run()
    assert(siModel.getFreq('I') == 1)
    assert(siModel.getFreq('S') == 4)
  }

  test("Infection SI-model birthRate=1") {
    val siModel = new SI {
      override val birthRate = 1.0
    }
    siModel.run()
    assert(siModel.getFreq('I') == 5)
    assert(siModel.getFreq('S') == 0)
  } // test SI

  test("Infection SI-model 1-Iteration") {
    val siModel = new SI {
      override val trial = 1
      override val birthRate = 1.0
    }
    siModel.run()
    assert(siModel.getFreq('I') == 2)
    assert(siModel.getFreq('S') == 3)
  }

  test("Infection SI-model 2-Iteration") {
    val siModel = new SI {
      override val trial = 2
      override val birthRate = 1.0
    }
    siModel.run()
    assert(siModel.getFreq('I') == 4)
    assert(siModel.getFreq('S') == 1)
  }
}
