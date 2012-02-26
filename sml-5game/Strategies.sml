(* evaluate board *)

exception IllegalFormat;

val nth = List.nth

fun evalBoard6 brd =
    foldl (fn (ptn,x) =>
              case ptn of
                  [W,W,W,N,N,_] => ~1+x
                | [W,W,N,W,N,_] => ~1+x
                | [W,N,W,W,N,_] => ~1+x
                | [N,W,W,W,N,_] => ~1+x
                | [W,W,N,N,W,_] => ~1+x
                | [W,N,W,N,W,_] => ~1+x
                | [N,W,W,N,W,_] => ~1+x
                | [W,N,N,W,W,_] => ~1+x
                | [N,W,N,W,W,_] => ~1+x
                | [N,N,W,W,W,_] => ~1+x

                | [_,W,W,W,N,N] => ~1+x
                | [_,W,W,N,W,N] => ~1+x
                | [_,W,N,W,W,N] => ~1+x
                | [_,N,W,W,W,N] => ~1+x
                | [_,W,W,N,N,W] => ~1+x
                | [_,W,N,W,N,W] => ~1+x
                | [_,N,W,W,N,W] => ~1+x
                | [_,W,N,N,W,W] => ~1+x
                | [_,N,W,N,W,W] => ~1+x
                | [_,N,N,W,W,W] => ~1+x

                | [B,B,B,N,N,_] => 1+x
                | [B,B,N,B,N,_] => 1+x
                | [B,N,B,B,N,_] => 1+x
                | [N,B,B,B,N,_] => 1+x
                | [B,B,N,N,B,_] => 1+x
                | [B,N,B,N,B,_] => 1+x
                | [N,B,B,N,B,_] => 1+x
                | [B,N,N,B,B,_] => 1+x
                | [N,B,N,B,B,_] => 1+x
                | [N,N,B,B,B,_] => 1+x

                | [_,B,B,B,N,N] => 1+x
                | [_,B,B,N,B,N] => 1+x
                | [_,B,N,B,B,N] => 1+x
                | [_,N,B,B,B,N] => 1+x
                | [_,B,B,N,N,B] => 1+x
                | [_,B,N,B,N,B] => 1+x
                | [_,N,B,B,N,B] => 1+x
                | [_,B,N,N,B,B] => 1+x
                | [_,N,B,N,B,B] => 1+x
                | [_,N,N,B,B,B] => 1+x

                | [W,W,W,W,N,_] => ~10+x
                | [W,W,W,N,W,_] => ~10+x
                | [W,W,N,W,W,_] => ~10+x
                | [W,N,W,W,W,_] => ~10+x
                | [N,W,W,W,W,_] => ~10+x
                | [_,W,W,W,W,N] => ~10+x
                | [_,W,W,W,N,W] => ~10+x
                | [_,W,W,N,W,W] => ~10+x
                | [_,W,N,W,W,W] => ~10+x
                | [_,N,W,W,W,W] => ~10+x

                | [B,B,B,B,N,_] => 10+x
                | [B,B,B,N,B,_] => 10+x
                | [B,B,N,B,B,_] => 10+x
                | [B,N,B,B,B,_] => 10+x
                | [N,B,B,B,B,_] => 10+x
                | [_,B,B,B,B,N] => 10+x
                | [_,B,B,B,N,B] => 10+x
                | [_,B,B,N,B,B] => 10+x
                | [_,B,N,B,B,B] => 10+x
                | [_,N,B,B,B,B] => 10+x
                | _ => 0+x)
    0 (mk6list brd)
and mk6list brd =
    let val brd' = fillZeroBoard brd in
        diagonalList6 brd'
    end
and diagonalList6 [h00::h01::h02::h03::h04::h05::h06::nil,
                   h10::h11::h12::h13::h14::h15::h16::nil,
                   h20::h21::h22::h23::h24::h25::h26::nil,
                   h30::h31::h32::h33::h34::h35::h36::nil,
                   h40::h41::h42::h43::h44::h45::h46::nil,
                   h50::h51::h52::h53::h54::h55::h56::nil,
                   h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h10,h21,h32,h43,h54,h65],
     [h01,h12,h23,h34,h45,h56],
     [h50,h41,h32,h23,h14,h05],
     [h61,h52,h43,h34,h25,h16]
    ]
  | diagonalList6 _ = raise IllegalFormat
        
fun evalBoard7 brd =
    foldl (fn (ptn,x) =>
              case ptn of
                  [W,W,W,N,N,_,_] => ~1+x
                | [_,W,W,W,N,N,_] => ~1+x
                | [_,_,W,W,W,N,N] => ~1+x
                | [B,B,B,N,N,_,_] => 1+x
                | [_,B,B,B,N,N,_] => 1+x
                | [_,_,B,B,B,N,N] => 1+x

                | [B,B,B,N,B,_,_] => 5+x
                | [B,B,N,B,B,_,_] => 5+x
                | [B,N,B,B,B,_,_] => 5+x
                | [N,B,B,B,B,_,_] => 5+x
                | [_,B,B,B,N,B,_] => 5+x
                | [_,B,B,N,B,B,_] => 5+x
                | [_,B,N,B,B,B,_] => 5+x
                | [_,N,B,B,B,B,_] => 5+x
                | [_,_,B,B,B,N,B] => 5+x
                | [_,_,B,B,N,B,B] => 5+x
                | [_,_,B,N,B,B,B] => 5+x
                | [_,_,N,B,B,B,B] => 5+x

                | [W,W,W,N,W,_,_] => ~5+x
                | [W,W,N,W,W,_,_] => ~5+x
                | [W,N,W,W,W,_,_] => ~5+x
                | [N,W,W,W,W,_,_] => ~5+x
                | [_,W,W,W,N,W,_] => ~5+x
                | [_,W,W,N,W,W,_] => ~5+x
                | [_,W,N,W,W,W,_] => ~5+x
                | [_,N,W,W,W,W,_] => ~5+x
                | [_,_,W,W,W,N,W] => ~5+x
                | [_,_,W,W,N,W,W] => ~5+x
                | [_,_,W,N,W,W,W] => ~5+x
                | [_,_,N,W,W,W,W] => ~5+x

                | _ => 0+x)
    0 (mk7list brd)
and mk7list brd =
    let val brd' = fillZeroBoard brd in
        brd' @ horizontalList brd' @ diagonalList brd'
    end
and horizontalList [nil,nil,nil,nil,nil,nil,nil] = nil
  | horizontalList [h0::t0,h1::t1,h2::t2,h3::t3,h4::t4,h5::t5,h6::t6] =
    [h0,h1,h2,h3,h4,h5,h6]::horizontalList [t0,t1,t2,t3,t4,t5,t6]
  | horizontalList _ = raise IllegalFormat
and diagonalList [h00::h01::h02::h03::h04::h05::h06::nil,
                  h10::h11::h12::h13::h14::h15::h16::nil,
                  h20::h21::h22::h23::h24::h25::h26::nil,
                  h30::h31::h32::h33::h34::h35::h36::nil,
                  h40::h41::h42::h43::h44::h45::h46::nil,
                  h50::h51::h52::h53::h54::h55::h56::nil,
                  h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h00,h11,h22,h33,h44,h55,h66],
     [h66,h55,h44,h33,h22,h11,h00],
     [h60,h51,h42,h33,h24,h15,h06],
     [h06,h15,h24,h33,h42,h51,h60]
    ]
  | diagonalList _ = raise IllegalFormat

fun evalBoard5 brd =
    foldl (fn (ptn,x) =>
              case ptn of
                  [W,W,W,W,N] => ~10+x
                | [W,W,W,N,W] => ~10+x
                | [W,W,N,W,W] => ~10+x
                | [W,N,W,W,W] => ~10+x
                | [N,W,W,W,W] => ~10+x

                | [B,B,B,B,N] => 10+x
                | [B,B,B,N,B] => 10+x
                | [B,B,N,B,B] => 10+x
                | [B,N,B,B,B] => 10+x
                | [N,B,B,B,B] => 10+x

                | _           => 0+x)
          0 (mk5list brd)
and diagonalList5 [h00::h01::h02::h03::h04::h05::h06::nil,
                   h10::h11::h12::h13::h14::h15::h16::nil,
                   h20::h21::h22::h23::h24::h25::h26::nil,
                   h30::h31::h32::h33::h34::h35::h36::nil,
                   h40::h41::h42::h43::h44::h45::h46::nil,
                   h50::h51::h52::h53::h54::h55::h56::nil,
                   h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h02,h13,h24,h35,h46],
     [h20,h31,h42,h53,h64],
     [h40,h31,h22,h13,h04],
     [h62,h53,h44,h35,h26]
    ]
  | diagonalList5 _ = raise IllegalFormat
and mk5list brd =
    let val brd' = fillZeroBoard brd in
        diagonalList5 brd'
    end

(*
== patternK ==

    | puts white
    v

   NNWWWN
    N   N
        = <- three black not here
        =
        =
*)

fun countWB (W::xs) = ~1 + countWB xs
  | countWB (B::xs) = 1  + countWB xs
  | countWB (N::xs) = countWB xs
  | countWB nil     = 0

fun evalBoardK brd =
    let val brd' = fillZeroBoard brd in
        5 * (foldl (fn (ptn,x) => countWB ptn + x)
                   0 (patternK brd'))
    end    
and patternK [h00::h01::h02::h03::h04::h05::h06::nil,
              h10::h11::h12::h13::h14::h15::h16::nil,
              h20::h21::h22::h23::h24::h25::h26::nil,
              h30::h31::h32::h33::h34::h35::h36::nil,
              h40::h41::h42::h43::h44::h45::h46::nil,
              h50::h51::h52::h53::h54::h55::h56::nil,
              h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h20,h21,h22,h23,h24,h25,h26,h40,h41,h42,h43,h44,h45,h46]
    ]
  | patternK _ = raise IllegalFormat


fun evalBoardK2 brd =
    let val brd' = fillZeroBoard brd in
        8 * (foldl (fn (ptn,x) => countWB ptn + x)
                   0 (patternK2 brd'))
    end    
and patternK2 [h00::h01::h02::h03::h04::h05::h06::nil,
              h10::h11::h12::h13::h14::h15::h16::nil,
              h20::h21::h22::h23::h24::h25::h26::nil,
              h30::h31::h32::h33::h34::h35::h36::nil,
              h40::h41::h42::h43::h44::h45::h46::nil,
              h50::h51::h52::h53::h54::h55::h56::nil,
              h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h30,h31,h32,h33,h34,h35,h36]
    ]
  | patternK2 _ = raise IllegalFormat

    (* None,None,White,White,White,None,None,None *)

fun evalBoardK2L brd =
    let val brd' = fillZeroBoard brd in
        foldl (fn (ptn,x) =>
                  case ptn of
                      [N,N,W,W,W,N,N,W,N,B,B,B,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,W,B,B,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,B,W,B,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,B,B,W,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,W,W,B,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,W,B,W,N,B] => ~30+x
                    | [N,N,W,W,W,N,N,W,N,B,W,W,N,B] => ~30+x
                    | [N,N,B,B,B,N,N,B,N,W,W,W,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,B,W,W,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,W,B,W,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,W,W,B,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,B,B,W,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,B,W,B,N,W] => 30+x
                    | [N,N,B,B,B,N,N,B,N,W,B,B,N,W] => 30+x
                    | _                             => 0+x
              )
              0 (patternK2L brd')
    end    
and patternK2L [h00::h01::h02::h03::h04::h05::h06::nil,
                h10::h11::h12::h13::h14::h15::h16::nil,
                h20::h21::h22::h23::h24::h25::h26::nil,
                h30::h31::h32::h33::h34::h35::h36::nil,
                h40::h41::h42::h43::h44::h45::h46::nil,
                h50::h51::h52::h53::h54::h55::h56::nil,
                h60::h61::h62::h63::h64::h65::h66::nil] =
    [
     [h01,h11,h21,h31,h41,h51,h61,h00,h10,h20,h30,h40,h50,h60],
     [h02,h12,h22,h32,h42,h52,h62,h01,h11,h21,h31,h41,h51,h61],
     [h03,h13,h23,h33,h43,h53,h63,h02,h12,h22,h32,h42,h52,h62]
    ]
  | patternK2L _ = raise IllegalFormat
    (* None,None,White,White,White,None,None,None *)

fun evalBoard brd =
    (* entire board *)
    (* 7 elems *)
    let val r = evalBoardK brd in
        let val r2 = evalBoardK brd in
            let val res7 = evalBoard7 brd in
                (* 6 elems *)
                let val res6 = evalBoard6 brd in
                    (* 5 elems *)
                    let val res5 = evalBoard5 brd in
                        r+r2+res7+res6+res5
                    end
                end
            end
        end
    end
