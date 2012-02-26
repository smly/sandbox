import System.Posix.Unistd
import System.Posix.Signals
import System.Exit

hello = do
  putStrLn "Hello, world1"
  exitWith (ExitFailure 1)

main = do
  oldHandler <- installHandler sigINT Ignore Nothing
  putStrLn "sleep 2"
  sleep 2
  oldHandler <- installHandler sigINT (Catch hello) Nothing
  putStrLn "sleep 1000000000"
  sleep 1000000000
  putStrLn "Hello, world!"
  exitWith ExitSuccess
