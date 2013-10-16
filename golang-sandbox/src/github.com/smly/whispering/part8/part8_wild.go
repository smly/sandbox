package main

/*
	Usage:
		$ go run part7/part7.go -private -dial localhost:4000 -port 4001
		$ go run part7/part7.go -private -dial localhost:4001 -port 4000
*/
import (
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"strings"
	"sync"
	"time"

	"code.google.com/p/whispering-gophers/util"
)

const (
	STATUS_REPORT_INTERVAL = 20
)

type Message struct {
	ID   string
	Addr string
	Body string
}

type TrackMessages struct {
	seen map[string]bool
	mu   sync.RWMutex
}

var tracking = &TrackMessages{
	seen: make(map[string]bool)}

func (t *TrackMessages) Size() int {
	t.mu.Lock()
	defer t.mu.Unlock()
	return len(t.seen)
}

func (t *TrackMessages) Seen(id string) bool {
	t.mu.Lock()
	defer t.mu.Unlock()
	if _, ok := t.seen[id]; ok {
		return true
	} else {
		t.seen[id] = true
		return false
	}
}

type Peers struct {
	m  map[string]chan<- Message
	mu sync.RWMutex
}

var peers = &Peers{
	m: make(map[string]chan<- Message)}

func (p *Peers) Add(addr string) <-chan Message {
	p.mu.Lock()
	defer p.mu.Unlock()
	if _, ok := p.m[addr]; ok {
		return nil
	} else {
		ch := make(chan Message)
		p.m[addr] = ch
		return ch
	}
}

func (p *Peers) Remove(addr string) {
	p.mu.Lock()
	defer p.mu.Unlock()
	delete(p.m, addr)
}

func (p *Peers) List() []chan<- Message {
	p.mu.Lock()
	defer p.mu.Unlock()
	var chs []chan<- Message
	for _, ch := range p.m {
		chs = append(chs, ch)
	}
	return chs
}

func (p *Peers) NameList() []string {
	p.mu.Lock()
	defer p.mu.Unlock()
	var names []string
	for addr, _ := range p.m {
		names = append(names, addr)
	}
	return names
}

var (
	dialAddr    = flag.String("dial", "0.0.0.0:4001", "dial address")
	listenPort  = flag.Int("port", 4000, "listen port")
	privateFlag = flag.Bool("private", false, "private network")
	wildFlag    = flag.Bool("wild", false, "ignore message id")
)
var self string

func Broadcast(msg Message) {
	if *wildFlag {
		msg.ID = util.RandomID()
	}
	for _, ch := range peers.List() {
		select {
		case ch <- msg: // Send message into a channel
		default: // Drop message
		}
	}
}

func Serve(conn net.Conn) {
	defer conn.Close()
	dec := json.NewDecoder(conn)
	for {
		var msg Message
		if err := dec.Decode(&msg); err != nil {
			log.Println(err)
			break
		}
		// Connect the peer when it send some message
		go Dialing(msg.Addr)
		// Track the ID of each received message
		if seen := tracking.Seen(msg.ID); !seen && *wildFlag {
			Broadcast(msg)
		}
	}
}

func LoopMessage() {
	for {
		var msg Message
		msg = Message{
			ID:   util.RandomID(),
			Addr: self,
			Body: "進捗どうですか？",
		}
		Broadcast(msg)
		time.Sleep(200 * time.Millisecond)
		msg = Message{
			ID:   util.RandomID(),
			Addr: self,
			Body: "進捗ダメです",
		}
		Broadcast(msg)
		time.Sleep(200 * time.Millisecond)
	}
}

func Communicate(out io.Writer, ch <-chan Message) {
	enc := json.NewEncoder(out)
	for msg := range ch {
		err := enc.Encode(msg)
		if err != nil {
			log.Println("Decode error found: ", err)
			return
		}
	}
}

func Dialing(addr string) {
	// localhost -> 127.0.0.1 -> 0.0.0.0
	switch {
	case strings.HasPrefix(addr, "localhost:"):
		elems := strings.Split(addr, ":")
		if len(elems) == 2 {
			addr = fmt.Sprintf("0.0.0.0:%s", elems[1])
		}
	case strings.HasPrefix(addr, "127.0.0.1:"):
		elems := strings.Split(addr, ":")
		if len(elems) == 2 {
			addr = fmt.Sprintf("0.0.0.0:%s", elems[1])
		}
	}

	switch {
	case addr == self:
		return
	case addr == "":
		return
	}

	ch := peers.Add(addr)
	if ch == nil {
		// already connected
		return
	}

	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Printf("Connection error: %s\n", err)

	} else {
		defer conn.Close()
		Communicate(conn, ch)
	}
	peers.Remove(addr)
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

func ConnectionStatus() {
	for {
		time.Sleep(STATUS_REPORT_INTERVAL * time.Second)
		log.Printf("Connected peers: %d, Tracking messages: %d\n",
			len(peers.NameList()),
			tracking.Size())

		for i, addr := range peers.NameList() {
			log.Printf("  - Peer%02d: %s\n", i, addr)
		}
	}

}

func Listen() (l net.Listener, err error) {
	if *privateFlag {
		return net.Listen("tcp", fmt.Sprintf(":%d", *listenPort))
	} else {
		return util.Listen()
	}
}

func main() {
	flag.Parse()

	go ConnectionStatus()
	go LoopMessage()
	go Dialing(*dialAddr)
	Listening()
}
