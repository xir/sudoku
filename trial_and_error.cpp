#include<iostream>
#include<cstdlib>
#include<deque>
#define side 9
std::deque<int> zeros;

bool checkRow(int pos, char val, char * grid)
{
  int start_pos = (pos/side)*side;
  for (int i=start_pos;i<start_pos+side;++i)
    if (grid[i] == val)
      return false;
  return true;
}

bool checkCol(int pos, char val, char * grid)
{
  int start_pos = pos%side;
  for (int i=start_pos;i<side*side;i+=side)
    if (grid[i] == val)
      return false;
  return true;
}

bool checkBox(int pos, char val, char * grid)
{
  int start_pos=(pos/(3*side))*3*side+((pos%side)/3)*3;
  for (int i=start_pos;i<start_pos+(3*side);i+=side)
    for (int j=i;j<i+3;++j)
      if (grid[j] == val)
        return false;
  return true;
}

bool isCorrect(int pos, char val, char * g)
{
  if (!(checkRow(pos,val,g) && checkCol(pos,val,g) && checkBox(pos,val,g)))
      return false;
  return true;
}

bool rSolve(char *g,int n)
{
  if (n>=zeros.size())
    return true;
  for (char c='1';c<='9';++c)
  {
    int pos = zeros[n];
    if (isCorrect(pos,c,g))
    {
      g[pos]=c;
      int start_pos= (pos/side)*side;
      if (rSolve(g,n+1)==1)
        return true;
      g[pos]='0';
    }
  }
  return false;
}

int main(int argc, char * argv[])
{
  int NUM_PUZZLES=1;
  int count=0;
  if (argc>1)
  {
    int t = atoi(argv[1]);
    if (t < 1 || t > 10000)
      NUM_PUZZLES = 1;
    else
      NUM_PUZZLES = t;
  }

  char grid[81];
  for (int i=1;i<=NUM_PUZZLES;++i)
  {
    for (int i=0;i<81;++i)
      std::cin >> grid[i];

    zeros.clear();
    for (int i=0; i<side*side;++i)
      if (grid[i]=='0')
        zeros.push_back(i);

    rSolve(grid,0);
    for (int i=0;i<side;++i)
    {
      for (int j=0;j<side;++j)
        std::cout << grid[i*side+j];
      std::cout << "\n";
    }
  }
  return 0;
}
