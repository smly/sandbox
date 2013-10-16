package main

import (
	"bufio"
	"encoding/json"
	"flag"
	"io"
	"log"
	"net"
	"os"
)

type Message struct {
	Body string
}

var address = flag.String("address", "localhost:4000", "address string")

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

func main() {
	flag.Parse()

	conn, err := net.Dial("tcp", *address)
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
