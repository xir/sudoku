#include"sudoku.h"
#include<iostream>
#include<cstdlib>
const int SIDE = 9;

int main(int argc, char * argv[])
{
  int NUM_PUZZLES=1;
  if (argc>1)
  {
    int t = atoi(argv[1]);
    if (t < 1 || t > 10000)
      NUM_PUZZLES = 1;
    else
      NUM_PUZZLES = t;
  }

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
