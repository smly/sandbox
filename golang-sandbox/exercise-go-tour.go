// ref: http://tour.golang.org/
package main

import (
	"code.google.com/p/go-tour/tree"
	"fmt"
	"image"
	"image/color"
	"io"
	"math"
	"math/cmplx"
	"net/http"
	"os"
	"strings"
)

// Tour 24: Exercise: Loops and Functions
func NewtonMethod(x float64) float64 {
	z := 0.0
	next_z := 0.0
	stop_diff := 1e-8
	for {
		next_z = z - (z*z-x)/(2*x)
		if math.Abs(z-next_z) < stop_diff {
			break
		}
		z = next_z
	}
	return next_z
}

// Tour 36: Exercise: Slices
func PreparePic(dx, dy int) [][]uint8 {
	rows := make([][]uint8, dy)
	for i := range rows {
		rows[i] = make([]uint8, dx)
	}
	return rows
}
func Pic(dx, dy int) [][]uint8 {
	rows := PreparePic(dx, dy)
	for i := range rows {
		for j := range rows[i] {
			rows[i][j] = uint8((i + j) / 2)
		}
	}
	return rows
}

// Tour 41: Exercise: Maps
func WordCount(s string) map[string]int {
	freq := make(map[string]int)
	for _, word := range strings.Fields(s) {
		freq[word] += 1
	}
	return freq
}

// Tour 44: Exercise: Fibonacci closure
func fibonacci() func() int {
	var pre, prepre int = 0, 0
	return func() (next int) {
		if prepre == 0 && pre == 0 {
			prepre = 1
			return 0
		}
		next = pre + prepre
		prepre, pre = pre, next
		return
	}
}

// Tour 48: Advanced Exercise: Complex cube roots
func Cbrt(x complex128) complex128 {
	var err float64 = 1e-8
	var z, pre complex128 = x, 0
	for cmplx.Abs(z-pre) > err {
		pre = z
		z = z - (z*z*z-x)/(3*z*z)
	}
	return z
}

// Tour 52: Methods with pointer receivers
type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

// Tour 56: Exercise: Errors
type ErrNegativeSqrt float64

func (e ErrNegativeSqrt) Error() string {
	return fmt.Sprintf("cannot Sqrt negative number: %f",
		float64(e))
}
func Sqrt(f float64) (float64, error) {
	if f < 0 {
		return 0, ErrNegativeSqrt(f)
	}

	return math.Sqrt(f), nil
}

// Tour 58:
type Struct struct {
	Greeting string
	Punct    string
	Who      string
}

type String string

func (h *Struct) ServeHTTP(
	w http.ResponseWriter,
	r *http.Request) {
	fmt.Fprintf(w, "G: %s, P: %s, W: %s\n", h.Greeting, h.Punct, h.Who)
}
func (s String) ServeHTTP(
	w http.ResponseWriter,
	r *http.Request) {
	fmt.Fprintf(w, string(s))
}

// Tour 60: Exercise: Images
type Image struct {
	Width, Height int
}

func (im *Image) ColorModel() color.Model {
	return color.RGBAModel
}
func (im *Image) Bounds() image.Rectangle {
	return image.Rect(0, 0, im.Width, im.Height)
}
func (im *Image) At(x, y int) color.Color {
	return color.RGBA{uint8(x), uint8(y), 155, 255}
}

// Tour 61: Exercise: Rot13 Reader
type rot13Reader struct {
	r io.Reader
}

func (rot *rot13Reader) Read(p []byte) (n int, err error) {
	n, err = rot.r.Read(p)
	for i := range p {
		switch {
		case p[i] >= 'A' && p[i] <= 'M':
			p[i] += 13
		case p[i] >= 'a' && p[i] <= 'm':
			p[i] += 13
		case p[i] >= 'N' && p[i] <= 'Z':
			p[i] -= 13
		case p[i] >= 'n' && p[i] <= 'z':
			p[i] -= 13
		}
	}
	return
}

// Tour 66: Range and Close
func fibonacciGen(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < n; i++ {
		c <- x
		x, y = y, x+y
	}
	close(c)
}

// Tour 70: Exercise: Equivalent Binary Trees
func RecursiveWalk(t *tree.Tree, ch chan int) {
	if t.Left != nil {
		RecursiveWalk(t.Left, ch)
	}
	ch <- t.Value
	if t.Right != nil {
		RecursiveWalk(t.Right, ch)
	}
}
func Walk(t *tree.Tree, ch chan int) {
	RecursiveWalk(t, ch)
	close(ch)
}
func Same(lhsTree, rhsTree *tree.Tree) bool {
	lhsChan := make(chan int)
	rhsChan := make(chan int)
	go Walk(lhsTree, lhsChan)
	go Walk(rhsTree, rhsChan)

	for i := range lhsChan {
		if i != <-rhsChan {
			return false
		}
	}
	return true
}

// Tour 71: Exercise: Web Crawler
// skip

// Entry point
func main() {
	for i := 3; i < 8; i += 1 {
		fmt.Println("Original math.Sqrt(i): ", math.Sqrt(float64(i)))
		fmt.Println("Newton sqrt(i): ", NewtonMethod(float64(i)))

		// Tour 48
		fmt.Println("Cbrt(2): ", Cbrt(2), ", cmplx.Pow(Cbrt(2), 3): ", cmplx.Pow(2, 3))
		fmt.Println("Cbrt(4): ", Cbrt(4), ", cmplx.Pow(Cbrt(4), 3): ", cmplx.Pow(4, 3))
		fmt.Println("Cbrt(5): ", Cbrt(5), ", cmplx.Pow(Cbrt(5), 3): ", cmplx.Pow(5, 3))
	}

	// Tour 58
	http.Handle("/string", String("I'm a frayed knot."))
	http.Handle("/struct", &Struct{"Hello", ":", "Gophers!"})
	//http.ListenAndServe("localhost:4000", nil)

	// Tour 61
	s := strings.NewReader(
		"Lbh penpxrq gur pbqr!\n")
	r := rot13Reader{s}
	io.Copy(os.Stdout, &r)

	// Tour 66
	fibChannel := make(chan int, 10)
	go fibonacciGen(cap(fibChannel), fibChannel)
	for i := range fibChannel {
		fmt.Print(i)
	}

	// Tour 70
	ch := make(chan int, 10)
	go Walk(tree.New(1), ch)
	for i := range ch {
		fmt.Println(i)
	}
	fmt.Println(Same(tree.New(1), tree.New(1)))
	fmt.Println(Same(tree.New(3), tree.New(5)))
}
