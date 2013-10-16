package main

/*
	Usage:
		$ go run part3/part3.go --address localhost:4001
		$ go run part4/part4.go --dial localhost:4001 --listen localhost:4000
		// say something in part4
		$ go run part2/part2.go --address localhost:4000
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
)

type Message struct {
	Body string
}

var (
	dialAddr   = flag.String("dial", "localhost:4000", "dial address")
	listenAddr = flag.String("listen", "localhost:4000", "listen address")
)

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
		msg := Message{scanner.Text()}

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

func Listening() {
	l, err := net.Listen("tcp", *listenAddr)
	if err != nil {
		log.Fatal(err)
	}
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

	go Listening()
	Dialing()
}
