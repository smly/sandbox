datatype 'a ternarylikeop = TrueExpression of 'a | FalseExpression

fun op ? (true, y) = TrueExpression y
  | op ? (_,    y) = FalseExpression
infix 6 ?

fun op :~ (TrueExpression x, _) = x
  | op :~ (FalseExpression, y)  = y
infix 6 :~

fun op+= (x,n) =
    let val _ = (x := !x + n) in x end

infix 8 +=

val x = ref 1;
val y = ref 2;
val test01 = ((true) ? x :~ y) += 1;

val a = 1;
val b = 2;
val test02 = ((true) ? a :~ b);

val test03 = ((true) ? "huga" :~ "hoge");

val s = ref "huha";
val t = ref "hoha";
val test04 = ((true) ? s :~ t);
