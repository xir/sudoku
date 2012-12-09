#include"sudoku.h"
#include<iostream>
const int SIDE = 9;
const int NUM_PUZZLES = 50;

int main()
{
  sudoku A (SIDE);
  int count=0;
  for (int i=1;i<=NUM_PUZZLES;++i)
  {
    A.read();
    A.pop_options();
    A.solve();
    if (A.isSolved())
      ++count;
    else
    {
      //if(i==27)
      {
        std::cout << i << "\n";
        A.print();
      }
    }
  }
  std::cout << count << "\n";
  return 0;
}
