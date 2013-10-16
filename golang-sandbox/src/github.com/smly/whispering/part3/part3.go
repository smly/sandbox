package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net"
)

type Message struct {
	Body string
}

var address = flag.String("address", "localhost:4000", "address string")

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

func main() {
	flag.Parse()

	l, err := net.Listen("tcp", *address)
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
