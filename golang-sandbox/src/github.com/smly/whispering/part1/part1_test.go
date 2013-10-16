package main

import (
	"bufio"
	"bytes"
	. "launchpad.net/gocheck"
	"strings"
	"testing"
)

func Test(t *testing.T) { TestingT(t) }

type MySuite struct{}

var _ = Suite(&MySuite{})

func (s *MySuite) TestCommunicate(c *C) {
	var tests = []struct {
		in, want string
	}{
		{"Test", "{\"Body\":\"Test\"}\n"},
		{"Whisper", "{\"Body\":\"Whisper\"}\n"},
	}
	for _, tc := range tests {
		scanner := bufio.NewScanner(strings.NewReader(tc.in))
		wb := new(bytes.Buffer)
		Communicate(scanner, wb)
		got := wb.String()
		c.Check(got, Equals, tc.want)
	}
}
