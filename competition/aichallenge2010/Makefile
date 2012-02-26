# The Makefile
#
# If you're using Windows and you don't know what this file is,
# don't worry about it. Just use Visual C++ Express Edition or
# Dev-C++ to work on your code.

MyTronBot: MyTronBot.o Map.o
	g++ -o MyTronBot MyTronBot.o Map.o

MyTronBot.o: MyTronBot.cc
	g++ -c -o MyTronBot.o MyTronBot.cc -O2

Map.o: Map.cc
	g++ -c -o Map.o Map.cc -O2

clean:
	rm MyTronBot test
	rm *.o

test: Map.o
	cd tests
	g++ -c -o tests/test_simple.o tests/test_simple.cc -I.
	g++ -o tests/test_simple tests/test_simple.o Map.o -lgtest
	./tests/test_simple
