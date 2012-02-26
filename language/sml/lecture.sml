(* tutorial *)

(* force type with type constraints *)
fun double x:real = x + x;

(* structured data types: tuples, records, lists *)
val it = (1,2,3);
val it = (2, 4, double);
val (x, y) = (double 2.0, double 4.0);

val ex = { name = "apple", age = 3 };
val exname = #name ex;

(* curried function / can define new functions by partial evaluation *)
fun pow m n : int =
    if n = 0 then 1
    else m * pow m (n - 1);
val pow2 = pow 2;

(* function composition / local declaration *)
fun add_double n =
    let
      fun add x:real = 1.0 + x;
      fun dob x:real = x * 2.0;
    in
      (dob o add) n
    end;

(* hiding declaration *)
local
  fun fibo 0 = 1
    | fibo 1 = 1
    | fibo n = fibo (n - 2) + fibo (n - 1);

  (* iterative solution *)
  fun fastfibo n : int =
      let
        fun fiboLoop a b 0 = a
          | fiboLoop a b n:int = fiboLoop b (a+b) (n-1)
      in
        fiboLoop 1 1 n
      end;
in
  fun f n = fibo n; (* todo: sml-mode.el doesn't supported the syntax? *)
  fun f2 n = fastfibo n;
end;

(* polymorphic function -- many forms; many types *)
fun last [x] = x
  | last nil = hd nil
  | last (x::xs) = last xs;

fun intLt (x:int) y = x < y;
fun realLt (x:real) y = x < y;

local
  fun partition (pivot, nil) (lessThan) = (nil, nil)
    | partition (pivot,y::ys) (lessThan) =
      let
        val (smalls, bigs) = partition (pivot, ys) (lessThan)
      in
        if (lessThan y pivot)
        then (y::smalls, bigs)
        else (smalls, y::bigs)
      end;
in
  fun quicksort nil lessThan     = nil
    | quicksort [x] lessThan     = [x]
    | quicksort (x::xs) lessThan =
      let
        val (smalls, bigs) = partition (x, xs) lessThan
      in
        quicksort smalls lessThan @ [x] @ quicksort bigs lessThan
      end;
end;

(* declaring a polymorphic type *)
type 'a hungaa = 'a * int

(* data types / Asize and Bsize are constructors *)
datatype ulevel = Freshman | Soph | Junior | Senior;
datatype glevel = MS | PhD;
datatype student = Undergrad of ulevel
                 | Grad of int * glevel;

(*
 syntax of patterns in clausal-form function declarations
 ref: http://www.cs.cmu.edu/afs/cs/local/sml/common/smlguide/smlnj.htm
*)
fun level (Undergrad _)   = "An undergrad"
  | level (Grad (_, MS))  = "An MS student"
  | level (Grad (_, PhD)) = "An PhD student"

(* polymorphic types *)
datatype 'a btree = Leaf of 'a | Node of 'a btree * 'a btree;
val it = Node (Leaf 1, Node (Leaf 2, Leaf 3));

(* exceptions *)
exception InvalidOperation of string;

local
  exception EmptyList;

  fun last_ [x] = x
    | last_ nil = raise EmptyList
    | last_ (x::xs) = last_ xs
in
  fun positivelast x = last_ x
  handle EmptyList => ~1
       | _         => ~2;
end;

(* @@@ imperative features *)

(* toString *)
print(Int.toString(1) ^ "\n");

(* inputLine *)
(*
 TextIO.inputLine(TextIO.stdIn);
*)

(* reference variables *)
val pt = ref 17;
(* dereference with ! *)
pt := !pt + 3;

(* while loop *)
val counter = ref 1;
while !counter < 10 do (
  counter := !counter + 1;
  print(Int.toString(!counter)^" ")
);
