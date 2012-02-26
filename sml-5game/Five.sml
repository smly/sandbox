(* client *)

datatype status = WIN | LOSE | DRAW | stat of int list list
type move = int * int

exception CannotConnect
exception AlreadyClosed
exception UnknownStatus

structure Five : sig
  val connectToServer : string * int -> unit
  val close : unit -> unit
  val sendMove : move -> unit
  val recvBoard : unit -> status
end = struct
  (* common description for games *)
  functor mkClient( structure G:sig
    val sendMove  : move->int list
    val recvBoard : int list->status
    val getKeyPath: string
    val statLen   : int
  end ) = struct
    val socket = ref (NONE : (Socket.active INetSock.stream_sock option))
    val addr   = ref (NONE : (INetSock.sock_addr option))
    fun close() = if (isSome(!socket)) then (
                    Socket.close(valOf(!socket)) ;
                    socket := NONE; ())
                  else raise AlreadyClosed
    fun mkstring 0 = ""
    |   mkstring n = (str(chr 0))^(mkstring (n-1))
    fun adjustLen n s = let val l = size s in
        if (l < n) then s^(mkstring (n-l)) else String.extract(s,0,(SOME n))
      end
    fun vectorToString vec =
      Word8Vector.foldl(fn(a,b)=> b ^ Char.toString(chr(Word8.toInt a))) "" vec
    fun recvByte() = (fn n => if n >= 128 then (n-256) else n)
            (Word8.toInt(Word8Vector.sub(Socket.recvVec(valOf(!socket),1),0)))
    fun sendByte(d) = Socket.sendVec(valOf(!socket),
            (Word8VectorSlice.full(Word8Vector.fromList [(Word8.fromInt d)])))
    fun recvString() = let val l=recvByte() in
      vectorToString (Socket.recvVec(valOf(!socket), l))
    end
    fun sendString s = let val n = String.size s in (
      sendByte(n);
      Socket.sendVec(valOf(!socket),
                     (Word8VectorSlice.full(Word8Vector.fromList
                                    (map (fn x => Word8.fromInt(ord x))
                                                (explode(adjustLen n s)))))))
    end
    fun sendMove h = ( map (fn i => sendByte(i)) (G.sendMove h); ())
    fun recvBoard()= G.recvBoard (map (fn _ => recvByte())
                                      (List.tabulate(G.statLen,(fn _=>()))))
    fun getKey s =
      let val p = Posix.IO.pipe() in
        (fn NONE      => ( (* child process *)
           Posix.IO.close(Posix.FileSys.stdout);
           Posix.IO.dup2( { new = Posix.FileSys.stdout, old = (#infd p) } );
           Posix.IO.close(#infd p); Posix.IO.close(#outfd p);
           Posix.Process.exec(G.getKeyPath,["getKey",s]); "")
         |  SOME(pid) => ( (* parent process *)
           Posix.IO.close(#infd p);
           vectorToString(Posix.IO.readVec(#outfd p, 70)))
        )(Posix.Process.fork())
      end
    fun connectToServer(host,port) = (
      print("start\n");

      if (isSome(!socket)) then close() else ();
      addr   := SOME(INetSock.toAddr(((fn NONE => raise CannotConnect
                                       |  SOME(ad) => ad)
                                 (NetHostDB.fromString(host))),port));
      socket := SOME(INetSock.TCP.socket());
      Socket.connect(valOf(!socket),valOf(!addr));

      print("connect ok\n");

(* no need to auth for hacked server

      let val k = recvString() in
        print(k);
        let val m = getKey(k) in 
          ( sendString m )
        end
      end ;
*)
      ()
    );
  end


  (************************************************************
   *
   * specific decsription for the game: Five in 2008
   *
   ************************************************************)
  structure SpecGame = mkClient( structure G = struct
    (* local *)
    val BoardWidth = 7
    fun consInner x [] = [[x]]
    |   consInner x (y::ys) = ((x::y)::ys)
    fun separate _ _ [] = []
    |   separate n 0 b  = [] :: (separate n n b)
    |   separate n i (x::xs) = consInner x (separate n (i-1) xs)
    fun transStat n b = 
             (map (foldr (fn (x,xs) => if x = ~1 then xs else (x::xs))[])
                  (separate n n b))
    (* interface between specific game and common client routine
     * status                          ; type of game status
     * move                            ; type of move
     * sendMove  : move -> int list    ; type
     * recvBoard : int list -> status  ;
     * getKeyPath: string              ;
     * statLen   : int                 ;
     *)
    fun sendMove(x,y) = [x,y];
    fun recvBoard (0::_) = WIN
    |   recvBoard (1::_) = LOSE
    |   recvBoard (2::_) = DRAW
    |   recvBoard (c::b) = stat(transStat BoardWidth b)
    |   recvBoard _ = raise UnknownStatus
    val getKeyPath = "/home/pub/j-yamagu/5moku/getKey"
    val statLen = BoardWidth*BoardWidth+1
  end )

  (* exports *)
  val connectToServer = SpecGame.connectToServer
  val close           = SpecGame.close
  val sendMove        = SpecGame.sendMove
  val recvByte        = SpecGame.recvByte
  val recvBoard       = SpecGame.recvBoard
end
