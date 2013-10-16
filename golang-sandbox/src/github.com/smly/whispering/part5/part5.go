package main

/*
	Usage:
		$ go run part3/part3.go -address localhost:4001
		$ go run part5/part5.go -private -dial localhost:4001 -port 4000
		// say something in part5
		$ go run part2/part2.go -address localhost:4000
		// say something in part2
*/
import (
	"bufio"
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"

	"code.google.com/p/whispering-gophers/util"
)

type Message struct {
	Addr string
	Body string
}

var (
	dialAddr    = flag.String("dial", "localhost:4001", "dial address")
	listenPort  = flag.Int("port", 4000, "listen port (private only)")
	privateFlag = flag.Bool("private", false, "private network")
)
var self string

func Serve(c net.Conn) {
	dec := json.NewDecoder(c)
	for {
		var msg Message
		if err := dec.Decode(&msg); err != nil {
			log.Println(err)
			break
		}

		// Echo received message
		fmt.Println(msg)
	}
	c.Close()
}

func Communicate(scanner *bufio.Scanner, out io.Writer) {
	enc := json.NewEncoder(out)
	for scanner.Scan() {
		msg := Message{Body: scanner.Text()}

		// scanner -> [encoder] -> out
		err := enc.Encode(msg)
		if err != nil {
			log.Fatal(err)
		}
	}
}

func Dialing() {
	conn, err := net.Dial("tcp", *dialAddr)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(os.Stdin)
	Communicate(scanner, conn)
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
	conn.Close()
}

func Listen() (l net.Listener, err error) {
	if *privateFlag {
		return net.Listen("tcp", fmt.Sprintf(":%d", *listenPort))
	} else {
		return util.Listen()
	}
}

func Listening() {
	l, err := Listen()
	if err != nil {
		log.Fatal(err)
	}
	self = l.Addr().String()
	log.Println("Listening on", self)

	// Listen
	for {
		conn, err := l.Accept()
		if err != nil {
			log.Fatal(err)
		}
		// Launch a goroutine to handle each connection
		go Serve(conn)
	}
}

func main() {
	flag.Parse()

	go Dialing()
	Listening()
}
