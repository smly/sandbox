import qualified Data.ByteString as S
import qualified Data.ByteString.Char8 as SC
import System.IO.Posix.MMap
import System.Environment
import Data.Bits
import Data.List
import Data.Word

data Header = Header {
      mapPtr :: Word32,
      datPtr :: Word32,
      ptrSkip :: Word32,
      mapSkip :: Word32 }

get4BWord32 :: S.ByteString -> Word32
get4BWord32 s = fromInteger $ toInteger $
                shiftL byte4 24 +
                shiftL byte3 16 +
                shiftL byte2 8 +
                byte1
    where byte1, byte2, byte3, byte4 :: Word32
          [byte1, byte2, byte3, byte4] =
              map fromIntegral $ S.unpack (S.take 4 s)

get3BWord32 :: S.ByteString -> Word32
get3BWord32 s = fromInteger $ toInteger $
                shiftL byte3 16 +
                shiftL byte2 8 +
                byte1
    where byte1, byte2, byte3 :: Word32
          [byte1, byte2, byte3] =
              map fromIntegral $ S.unpack (S.take 3 s)

getHeader :: S.ByteString -> IO (Header)
getHeader s = do
  return $ Header { mapPtr     = mptr,
                    datPtr     = dptr,
                    ptrSkip    = pskip,
                    mapSkip    = mskip
                  }
  where
    mptr = get4BWord32 s
    dptr = get4BWord32 (S.drop 3 s)
    pskip = shiftL mptr 8
    mskip = shiftL dptr 8

getMapping :: S.ByteString -> [(Int,Int)]
getMapping s = filter (\(a,_) -> if a==0 then False else True)
               [ getMap (S.drop x s) | x <- [0,3..(S.length s)-3]]
    where getMap :: S.ByteString -> (Int,Int)
          getMap sdata = (fromInteger (toInteger byte1),
                      fromInteger (toInteger ((shiftL byte3 8) + byte2)))
              where byte1, byte2, byte3 :: Word32
                    [byte1, byte2, byte3] = map fromIntegral $ S.unpack (S.take 3 sdata)

getPointers :: [S.ByteString] -> [Header] -> [[Int]]
getPointers ss hs =
    map getPointer
    [S.take (fromIntegral ((ptrSkip h)-1)) s | (s,h) <- zip ss hs]
    where
      getPointer :: S.ByteString -> [Int]
      getPointer s = filter (\n -> n/=0)
                     [ fromIntegral (get3BWord32 (S.drop x s)) | x <- [3,6..(S.length s)-3] ]


getFileInfo :: S.ByteString -> IO ()
getFileInfo s = do
  putStrLn $ "filename: "++ filename
  S.writeFile filename (S.take size (S.drop ptr s))
      where
        ptr  = fromIntegral $ get4BWord32 s
        size = fromIntegral $ get4BWord32 (S.drop 4 s)
        filename = SC.unpack (S.take (ptr-16) (S.drop 16 s))

divide :: [S.ByteString] -> [[Int]] -> [(Int,Int)] -> IO ()
divide ss ps m =
    mapM_ (\(aid,fid) -> do
             getFileInfo (S.drop (((ps !! (aid-1)) !! (fid-1))*256) (ss !! (aid-1)))
          ) m

extractArchives :: [S.ByteString] -> [Header] -> IO ()
extractArchives ss hs = do
  getFileInfo (S.drop dat_start sdata)
  divide ss pointers mapping
    where sdata = head ss
          header = head hs
          map_start = fromIntegral (ptrSkip header)
          dat_start = fromIntegral (mapSkip header)
          mapping = getMapping $ S.take (dat_start - map_start) $ S.drop map_start sdata
          pointers = getPointers ss hs

main :: IO ()
main = do
  archives <- getArgs
  ss <- mapM unsafeMMapFile archives
  hs <- mapM getHeader ss
  extractArchives ss hs
