#include<iostream>
#include<set>

const int SIDE = 9;
const int NUM_PUZZLES = 50;

class sudoku
{
  public:
    sudoku(int);
    ~sudoku();
    void update_options();
    void reduce_options();
    void hidden();
    void push_options();
    void pop_options();
    void row_direct(int);
    void row_hidden(int);
    void column_direct(int);
    void column_hidden(int);
    void box_direct(int);
    void box_hidden(int);
    void print();
    void read();
    void solve();
    void filled();
    bool isSolved();

  private:
    int num_filled;
    bool solved;
    int side;
    char * grid;
    std::set<char> * options;
};

bool sudoku::isSolved()
{
  filled();
  return num_filled == side*side;
}

void sudoku::filled()
{
  num_filled=side*side;
  for (int i=0;i<side*side;++i)
    if (grid[i]=='0')
      --num_filled;
}

sudoku::sudoku (int side)
{
  this->side=side;
  grid = new char[side*side];
  options = new std::set<char>[side*side];
}

sudoku::~sudoku()
{
  delete[] grid;
  delete[] options;
}

void sudoku::pop_options()
{
  char a;
  for (int i=0;i<side*side;++i)
    if (grid[i] == '0' || grid[i] == '.')
    {
      a='1';
      for (int j=0;j<9;j++)
      {
        options[i].insert(a);
        a++;
      }
    }
}

void sudoku::reduce_options()
{
  for (int i=0;i<side*side;++i)
  {
    column_direct(i);
    row_direct(i);
    box_direct(i);
  }
}

void sudoku::hidden()
{
  for (int i=0;i<9;++i)
  {
    row_hidden(i);
    column_hidden(i);
  }
}

// takes an entry, removes it from the options list of that row
void sudoku::row_direct(int x)
{
  // first element in the row
  int start_pos = (x/side)*side;
  for (int i=start_pos;i<start_pos+side;++i)
  {
    char t = grid[x];
    options[i].erase(t);
  }
}

void sudoku::column_direct(int x)
{
  // first element in the column
  int start_pos = x%side;
  for (int i=start_pos;i<side*side;i+=side)
  {
    char t = grid[x];
    options[i].erase(t);
  }
}

void sudoku::column_hidden(int x)
{
  int startpos = x;
  int * tally = new int[side];
  for (int i=0;i<side;++i)
    tally[i]=0;
  for (int i=startpos;i<side*side;i+=side)
  {
    char a = '1';
    for (int j=0;j<9;++j)
    {
      if (options[i].count(a)==1)
        ++tally[j];
      ++a;
    }
  }
  for (int i=startpos;i<side*side;i+=side)
  {
    char a='1';
    for (int j=0;j<9;++j)
    {
      if (tally[j]==1 && options[i].count(a)==1)
      {
        options[i].clear();
        options[i].insert(a);
      }
      ++a;
    }
  }

}

void sudoku::row_hidden(int x)
{
  int startpos = x*side;
  int * tally = new int[side];
  for (int i=0;i<side;++i)
    tally[i]=0;
  for (int i=startpos;i<startpos+side;++i)
  {
    char a='1';
    for (int j=0;j<9;++j)
    {
      if (options[i].count(a)==1)
        ++tally[j];
      ++a;
    }
  }
  for (int i=startpos;i<startpos+side;++i)
  {
    char a='1';
    for (int j=0;j<9;++j)
    {
      if (tally[j]==1 && options[i].count(a)==1)
      {
        options[i].clear();
        options[i].insert(a);
      }
      ++a;
    }
  }
}

// hard coded for 9, oh well
void sudoku::box_direct(int x)
{
  int start_row = (x/(3*side))*3*side;
  int start_col = ((x%side)/3)*3;
  int start_pos = start_row + start_col;
  for (int i=start_pos;i<start_pos+(3*side);i+=side)
  {
    for (int j=0;j<3;++j)
    {
      char t = grid[x];
      options[i+j].erase(t);
    }
  }
}

void sudoku::push_options()
{
  std::set<char>::iterator it;
  for (int i=0;i<side*side;++i)
  {
    if (options[i].size() == 1)
    {
      it = (options[i]).begin();
      grid[i] = *it;
      options[i].clear();
    }
  }
}

void sudoku::read()
{
  delete[] grid;
  delete[] options;
  grid = new char[side*side];
  options = new std::set<char>[side*side];
  for (int i=0;i<side*side;++i)
  {
    std::cin >> grid[i];
  }
}

void sudoku::print()
{
  for (int i=0;i<side;++i)
  {
    for (int j=0; j<side;++j)
      std::cout << grid[i*side+j];
    std::cout << "\n";
  }
  std::cout << "\n";
}

void sudoku::solve()
{
  filled();
  if (num_filled != side*side)
    solved = false;
  else
    solved = true;
  int prev=num_filled;
  while(!solved)
  {
    reduce_options();
    hidden();
    push_options();

    filled();
    if (prev == num_filled)
      return;
    prev=num_filled;
  }
}

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
  }
  std::cout << count << "\n";
  return 0;
}
