
val s01 = [ [1,2,2,1,1,1,2],
            [1,2,1,1,1,1],
            [1],
            [1],
            [1],
            [1],
            [1]
          ]

val s02 = [[2,1,1,1,2],[],[2],[2,2,2],[2],[],[]]

(* test for patternK*)
val s03 = [[1],[],[2,1],[2,1],[2,1],[],[2]]

(* draw *)
val sample_board01 =
    [
     [W,B,B,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [B,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N]
    ]

(* B win / VerticalCheck *)
val sample_board02 =
    [
     [W,B,B,B,B,B,N],
     [W,N,N,N,N,N,N],
     [B,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N]
    ]

(* W win / horizontalCheck*)
val sample_board03 =
    [
     [W,W,B,B,B,B,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N],
     [W,N,N,N,N,N,N]
    ]

(* W win / diagonalCheck *)
val sample_board04 =
    [
     [W,W,B,B,B,B,N],
     [N,N,N,N,N,N,N],
     [W,W,N,N,N,N,N],
     [B,N,W,N,N,N,N],
     [W,N,N,W,N,N,N],
     [W,N,N,N,W,N,N],
     [W,N,N,N,N,W,N]
    ]

