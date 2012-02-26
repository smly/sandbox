import Data.List (tails,sort)
 
-- build suffix array
build :: Ord a => [a] -> [[a]]
build = sort.tails
 
-- binary search
search [] str  = False
search [x] str = cmp str x
search xs str
    | cmp str x = True
    | str < x   = search l str
    | otherwise = search r str
   where (l,x:r) = splitAt (length xs `div` 2) xs

-- partial compare
cmp str x = (length x >= length str) && (subcmp str x)
subcmp [] _ = True
subcmp (s:ss) (n:ns)
    | s == n = subcmp ss ns
    | otherwise = False

{-

*Main> let suffixarry = (build "atgcgcgtaagtctagtctagtatctgacatctactatcctgctttactctatgtcctgaatataagtagtatgtactagtcatgtatca")
*Main> :s +s
*Main> search suffixarry "atgc"
True
(0.01 secs, 559776 bytes)
*Main> search suffixarry "tagata"
False
(0.00 secs, 0 bytes)
*Main> search suffixarry "atg"
True
(0.00 secs, 0 bytes)

-}
