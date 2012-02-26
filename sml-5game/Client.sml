(* retrieve *)

use "Five.sml";
use "Board.sml";
use "Sample.sml";
use "Strategies.sml";

fun jiteban brd = let val j = judge brd in
                      if j<>0 then j else exists (next 1 brd)
                  end
and exists (f::l) = foldl (fn (a,b)=> let val j = aiteban a in
                                          if j>b then j else b
                                      end) (aiteban f) l
  |   exists []     = 0
and aiteban brd = let val j = judge brd in
                      if j<>0 then j else forall (next 2 brd)
                  end
and forall (f::l) = foldl (fn (a,b)=> let val j = jiteban a in
                                          if j<b then j else b
                                      end) (jiteban f) l
  |   forall []     = 0
and next p brd = map (fn f => (nextBoard 1 f brd)) (nextMoves brd)

fun jiteban2 n brd = let val j = judge brd in
                         if j<>0 then (~1,j*100) else 
                         if n=0 then (~1,evalBoard brd) else 
                         max n brd (nextMoves brd)
                     end
and max n brd (f::l) = foldl (fn (a,b)=>
                                 let val j = #2(aiteban2(n-1)(nextBoard 1 a brd)) in
                                     if j>(#2b) then (a,j) else b
                                 end) (f,#2(aiteban2(n-1)(nextBoard 1 f brd))) l
  |   max _ _ []     = (~1,0)
and aiteban2 n brd = let val j = judge brd in
                         if j<>0 then (~1,j*100) else 
                         if n=0 then (~1,evalBoard brd) else 
                         min n brd (nextMoves brd)
                     end
and min n brd (f::l) = foldl (fn (a,b)=>
                                 let val j = #2(jiteban2(n-1)(nextBoard 2 a brd)) in
                                     if j<(#2b) then (a,j) else b
                                 end) (f,#2(jiteban2(n-1)(nextBoard 2 f brd))) l
  |   min _ _ []     = (~1,0)

(*
fun jiteban3 n brd bt = let val j = judge brd in
                            if j<>0 then (~1,j*100) else 
                            if n=0 then (~1,evalBoard brd) else 
                            let val ms = nextMoves brd in 
                                if (null ms) then (~1,0) else
                                max1 n brd ms (~1,(~1000,bt)) bt
                            end
                        end
and max1 n brd (f::l) alh bt =
    let val j = #2(aiteban3(n-1)(nextBoard 1 f brd)(#2alh)) in
        if j >= bt then (~1,j) else
        if j>(#2alh) then (max1 n brd l (f,j) bt) else
        (max1 n brd l alh bt)
    end
  |   max1 _ _ [] alh _ = alh
and aiteban3 n brd al = let val j = judge brd in
                            if j<>0 then (~1,j*100) else 
                            if n=0 then (~1,evalBoard brd) else 
                            let val ms = nextMoves brd in
                                if (null ms) then (~1,0) else
                                min1 n brd ms (~1,1000) al
                            end
                        end
and min1 n brd (f::l) bth al =
    let val j = #2(jiteban3(n-1)(nextBoard 2 f brd)(#2bth)) in
        if j <= al then (~1,j) else
        if j<(#2bth) then (min1 n brd l (f,j) al) else
        (min1 n brd l bth al)
    end
  |   min1 _ _ [] bth _ = bth
*)

fun jiteban3 n brd bt = let val j = judge brd in
                            if j<>0 then (~1,j*100) else 
                            if n=0 then (~1,evalBoard brd) else 
                            let val ms = nextMoves brd in 
                                if (null ms) then (~1,0) else
                                max1 n brd ms (~1,~1000) bt
                            end
                        end
and max1 n brd (f::l) alh bt =
    let val j = #2(aiteban3(n-1)(nextBoard 1 f brd)(#2alh)) in
        if j >= bt then (~1,j) else
        if j>(#2alh) then (max1 n brd l (f,j) bt) else
        (max1 n brd l alh bt)
    end
  |   max1 _ _ [] alh _ = alh
and aiteban3 n brd al = let val j = judge brd in
                            if j<>0 then (~1,j*100) else 
                            if n=0 then (~1,evalBoard brd) else 
                            let val ms = nextMoves brd in
                                if (null ms) then (~1,0) else
                                min1 n brd ms (~1,1000) al
                            end
                        end
and min1 n brd (f::l) bth al =
    let val j = #2(jiteban3(n-1)(nextBoard 2 f brd)(#2bth)) in
        if j <= al then (~1,j) else
        if j<(#2bth) then (min1 n brd l (f,j) al) else
        (min1 n brd l bth al)
    end
  |   min1 _ _ [] bth _ = bth

val flag: bool ref = ref false;
fun think depth step brd =
    if (step = 1) then (0,3) else
    if (step = 2 andalso brd = [[1],[],[2,1],[2,1],[2,1],[],[]]) then (flag := true; (2,4)) else
    if (step = 3 andalso !flag) then (2,4) else
    let val (hand,evl) = jiteban3 depth brd 1000 in
        print ("----------\n") ;
        print ("step: " ^ Int.toString(step) ^ "\n") ;
        print ("depth:: " ^ Int.toString(depth) ^ "\n") ;
        printBoard brd ;
        printHand hand ;
        print ("eval: " ^ Int.toString(evl) ^ "\n") ;
        ((hand mod 10)-1, (hand div 10)-1)
    end

fun mainLoop i = (
    let val k = Five.recvBoard() in (
        ((fn WIN  => print "Win!\n"
           |  DRAW => print "Draw\n"
           |  LOSE => print "Lose...\n"
           |  stat(brd) => (
              let val st = Time.now() in
                  let val lst = map List.rev brd in
                      case i of
                          1  => Five.sendMove(think 3 i lst)
                        | 2  => Five.sendMove(think 3 i lst)
                        | 3  => Five.sendMove(think 3 i lst)
                        | 4  => Five.sendMove(think 3 i lst)
                        | 5  => Five.sendMove(think 3 i lst)
                        | 6  => Five.sendMove(think 4 i lst)
                        | 7  => Five.sendMove(think 4 i lst)
                        | 8  => Five.sendMove(think 4 i lst)
                        | 9  => Five.sendMove(think 4 i lst)
                        | 10 => Five.sendMove(think 5 i lst)
                        | 11 => Five.sendMove(think 5 i lst)
                        | 12 => Five.sendMove(think 5 i lst)
                        | 13 => Five.sendMove(think 6 i lst)
                        | 14 => Five.sendMove(think 7 i lst)
                        | 15 => Five.sendMove(think 8 i lst)
                        | 16 => Five.sendMove(think 8 i lst)
                        | _  => Five.sendMove(think 9 i lst) ;
                      print("time: " ^ Time.toString(Time.-(Time.now(),st)) ^ " sec\n") ;
                      mainLoop(i+1)
                  end
              end
              )
         ) k))
    end ;
    ()
);

fun start port = (
    Five.connectToServer("127.0.0.1",port);
    mainLoop(1) ;
    Five.close() )

(*
- think 3 2 [[2],[1,1,2],[1,1,2],[2,1,1,2,2,2],[],[],[]];
----------
step: 2
depth:: 3
board:
|2
|112
|112
|211222
|
|
|
hand: (4,4)
eval: 100
*)
