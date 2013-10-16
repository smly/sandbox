package main

import (
	"bufio"
	"encoding/json"
	"io"
	"log"
	"os"
)

type Message struct {
	Body string
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

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	Communicate(scanner, os.Stdout)
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
