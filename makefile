all: sudoku
	cat sudoku.txt | ./sudoku

sudoku: sudoku.cpp
	g++ -o sudoku sudoku.cpp

run:
	cat sudoku.txt | ./sudoku
