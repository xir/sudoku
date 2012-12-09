all: sudoku
	cat sudoku.txt | ./sudoku 50

sudoku: sudoku.cpp
	g++ -o sudoku main.cpp sudoku.cpp

run:
	cat sudoku.txt | ./sudoku 50

test:
	g++ -o test main.cpp sudoku.cpp
	cat lol.txt | ./test 1

clean:
	rm *.dat test*
