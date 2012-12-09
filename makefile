all: sudoku
	cat sudoku.txt | ./sudoku

sudoku: sudoku.cpp
	g++ -o sudoku main.cpp sudoku.cpp

run:
	cat sudoku.txt | ./sudoku

test: lol.txt
	cat lol.txt | ./sudoku
