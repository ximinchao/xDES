xDESTest: xDESTest.o xDESKeyManager.o
	g++ -g -o xDESTest xDESTest.o xDESKeyManager.o

xDESTest.o: xDESTest.cpp
	g++ -g -c xDESTest.cpp

xDESKeyManager.o: xDESKeyManager.cpp xDESKeyManager.h
	g++ -g -c xDESKeyManager.cpp

.PHONY: clean
clean:
	-rm -f xDESTest.o xDESKeyManager.o xDESTest
