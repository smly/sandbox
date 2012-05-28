name := "Sandbox"

version := "0.0.1"

scalaVersion := "2.9.1"

scalacOptions ++= Seq("-deprecation", "-unchecked")

javacOptions ++= Seq("-Xlint:unchecked", "-Xlint:deprecation")

mainClass in (Compile, packageBin) := Some("com.sparsegraph.sandbox.Main")

mainClass in (Compile, run) := Some("com.sparsegraph.sandbox.Main")

seq(assemblySettings: _*)

// DEPENDENCIES for test
libraryDependencies ++= Seq (
  "org.scalacheck" %% "scalacheck" % "1.9" % "test",
  "org.scalatest" %% "scalatest" % "1.7.2" % "test",
  "org.mockito" % "mockito-all" % "1.9.0" % "test"
)

libraryDependencies ++= Seq (
  "org.apache.commons" % "commons-math3" % "3.0",
  "com.orientechnologies" % "orientdb-core" % "1.0",
  "org.neo4j" % "neo4j-kernel" % "1.7"
)

logLevel := Level.Info

traceLevel := 5

//seq(ProguardPlugin.proguardSettings :_*)

//proguardOptions ++= Seq (
//    "-dontshrink -dontoptimize -dontobfuscate -dontpreverify -dontnote " +
//    "-ignorewarnings",
//    keepAllScala
//)
