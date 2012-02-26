(* judge *)

datatype piece = N | W | B

exception IllegalBoardFormat

fun mkSeed () = (Int.fromLarge (Time.toMicroseconds(Time.now()) mod 100), 12);

fun random st ed = 
    let val seed = mkSeed() in
        Random.randRange (st,ed) (Random.rand seed)
    end

fun shuffle nil = nil
  | shuffle [x] = x::nil
  | shuffle lst =
    let val pos = random 0 ((length lst)-1) in
        (List.nth (lst,pos))::shuffle (List.take (lst,pos) @ List.drop (lst,pos+1))
    end

fun nextBoard piece x brd = foldl (applyhand piece) brd ((x mod 10)-1::(x div 10)-1::nil)
and applyhand piece (x,brd) = (List.take (brd,x)) @ [(List.concat ((List.nth (brd,x))::[piece]::nil))] @ (List.drop (brd,x+1));

val hands = [(0,0),(0,1),(0,2),(0,3),(0,4),(0,5),(0,6),
             (1,1),(1,2),(1,3),(1,4),(1,5),(1,6),
             (2,2),(2,3),(2,4),(2,5),(2,6),
             (3,3),(3,4),(3,5),(3,6),
             (4,4),(4,5),(4,6),
             (5,5),(5,6),
             (6,6)]

fun lensCheck (l::ls) 0 size = if (l+size > 7) then false else true
  | lensCheck (l::ls) parent size = lensCheck ls (parent-1) size
  | lensCheck _ _ _ = raise IllegalBoardFormat

fun brdfilter brd lens (x,y) =
    if x = y then
        if (lensCheck lens x 2) then true else false
    else
        if ((lensCheck lens x 1) andalso (lensCheck lens y 1)) then true else false

fun lengthlist brd = map length brd

fun nextMoves brd = map (fn (x,y) => (x+1) + (y+1) * 10) (nextMove brd)
and nextMove brd = List.filter (brdfilter brd (lengthlist brd)) (shuffle hands)

fun changeData 0 = N
  | changeData 2 = W
  | changeData 1 = B
  | changeData _ = raise IllegalBoardFormat

fun fillZeroBoard_ (x::xs) n = (changeData x)::fillZeroBoard_ xs (n-1)
  | fillZeroBoard_ _ 0       = nil
  | fillZeroBoard_ nil n     = N::fillZeroBoard_ nil (n-1)

fun fillZeroBoard (x::xs) = (fillZeroBoard_ x 7)::(fillZeroBoard xs)
  | fillZeroBoard nil     = nil

fun convertBrd brd = List.concat ( fillZeroBoard brd )

fun checker l =
    foldl (fn ([W,W,W,W,W,_,_],_) => ~1
            | ([_,W,W,W,W,W,_],_) => ~1
            | ([_,_,W,W,W,W,W],_) => ~1
            | ([B,B,B,B,B,_,_],_) => 1
            | ([_,B,B,B,B,B,_],_) => 1
            | ([_,_,B,B,B,B,B],_) => 1
            | (_,x)               => x) 0
          l
fun horizontalCheck l = checker (horizontalCheck_ l)
and horizontalCheck_ [nil,nil,nil,nil,nil,nil,nil] = nil
  | horizontalCheck_ [h0::r0,h1::r1,h2::r2,h3::r3,h4::r4,h5::r5,h6::r6] =
    [h0,h1,h2,h3,h4,h5,h6]::horizontalCheck_ [r0,r1,r2,r3,r4,r5,r6]
  | horizontalCheck_ _ = raise IllegalBoardFormat

fun verticalCheck l = checker l

fun diagonalCheck l = checker_five (diagonalCheck_ l)
and diagonalCheck_ [h00::h01::h02::h03::h04::h05::h06::nil,
                    h10::h11::h12::h13::h14::h15::h16::nil,
                    h20::h21::h22::h23::h24::h25::h26::nil,
                    h30::h31::h32::h33::h34::h35::h36::nil,
                    h40::h41::h42::h43::h44::h45::h46::nil,
                    h50::h51::h52::h53::h54::h55::h56::nil,
                    h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h60,h51,h42,h33,h24],
     [h51,h42,h33,h24,h15],
     [h42,h33,h24,h15,h06],

     [h50,h41,h32,h23,h14],
     [h41,h32,h23,h14,h05],
     [h61,h52,h43,h34,h25],
     [h52,h43,h34,h25,h16],

     [h40,h31,h22,h13,h04],
     [h62,h53,h44,h35,h26],

     [h00,h11,h22,h33,h44],
     [h11,h22,h33,h44,h55],
     [h22,h33,h44,h55,h66],

     [h10,h21,h32,h43,h54],
     [h21,h32,h43,h54,h65],
     [h01,h12,h23,h34,h45],
     [h12,h23,h34,h45,h56],

     [h20,h31,h42,h53,h64],
     [h02,h13,h24,h35,h46]
    ]
                                                              
  | diagonalCheck_ _ = raise IllegalBoardFormat
and checker_five l =
    foldl (fn ([W,W,W,W,W],_) => ~1
            | ([B,B,B,B,B],_) => 1
            | (_,x)               => x) 0
          l

fun printBoard brd =
    (
     print "board:\n" ;
     map (fn l => (print ("|"); printLine l; print "\n")) brd
    )
and printLine l = map (fn elem => print (str(chr(elem+48)))) l

fun printHand hand =
    print ("hand: ("
           ^ Int.toString((hand mod 10)-1)
           ^ ","
           ^ Int.toString((hand div 10)-1)
           ^ ")\n") ;

fun judge brd =
    let val brd' = fillZeroBoard brd in
        let val res = horizontalCheck brd' in
            if res <> 0 then res else 
            let val res' = verticalCheck brd' in
                if res' <> 0 then res' else
                let val res'' = diagonalCheck brd' in
                    if res'' <> 0 then res'' else 0
                end
            end
        end
    end
