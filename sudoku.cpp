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
    void box();
    void push_options();
    void pop_options();
    void row_direct(int);
    void row_hidden(int);
    void column_direct(int);
    void column_hidden(int);
    void box_direct(int);
    void box_hidden(int);
    void box_row(int);
    void print();
    void read();
    void solve();
    void filled();
    bool isSolved();
    void countTally(int*,int);
    void remove_from_row(int,char);
    void remove_from_col(int,char);
    void print_options(int);
    void print_array(int*);

    void countBox(int*,int);

  //private:
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
      for (int j=0;j<side;j++)
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
  for (int i=0;i<side;++i)
  {
    row_hidden(i);
    column_hidden(i);
    box_hidden(i);
  }
}

void sudoku::box()
{
  for (int i=0;i<side;++i)
    box_row(i);
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
  int start_pos = x;
  int * tally = new int[side];
  for (int i=0;i<side;++i)
    tally[i]=0;
  for (int i=start_pos;i<side*side;i+=side)
    countTally(tally,i);

  for (int i=start_pos;i<side*side;i+=side)
  {
    char a='1';
    for (int j=0;j<side;++j)
    {
      if (tally[j]==1 && options[i].count(a)==1)
      {
        options[i].clear();
        options[i].insert(a);
      }
      ++a;
    }
  }
  delete[] tally;
}

void sudoku::row_hidden(int x)
{
  int start_pos = x*side;
  int * tally = new int[side];
  for (int i=0;i<side;++i)
    tally[i]=0;
  for (int i=start_pos;i<start_pos+side;++i)
    countTally(tally,i);

  for (int i=start_pos;i<start_pos+side;++i)
  {
    char a='1';
    for (int j=0;j<side;++j)
    {
      if (tally[j]==1 && options[i].count(a)==1)
      {
        options[i].clear();
        options[i].insert(a);
      }
      ++a;
    }
  }
  delete[] tally;
}

void sudoku::box_hidden(int x)
{
  int start_pos=(x/3)*3*side+(x%3)*3;
  int * tally = new int[side];
  for (int i=0;i<side;++i)
    tally[i]=0;
  countBox(tally,start_pos);

  for (int i=start_pos;i<start_pos+(3*side);i+=side)
  {
    for (int j=i;j<i+3;++j)
    {
      char a='1';
      for (int k=0;k<side;++k)
      {
        if (tally[k]==1 && options[j].count(a)==1)
        {
          options[j].clear();
          options[j].insert(a);
        }
        ++a;
      }
    }
  }
  delete[] tally;
}

// puts the number of each possibility 1-9 in 0-8 of tally
void sudoku::countBox(int * tally, int start_pos)
{
  for (int i=start_pos;i<start_pos+(3*side);i+=side)
    for (int j=i;j<i+3;++j)
      countTally(tally,j);
}

// examine the rows within a box
// if total options of a number are the same in the box and in the row
// then number can only occur in that row, therefore it can't occur in the
// other two rows of that box, so remove them
// edits options outside of its box
void sudoku::box_row(int x)
  // not working
{
  int start_pos=(x/3)*3*side+(x%3)*3;
  int * tally = new int[side];
  int * row_tally = new int[side];

  for (int i=0;i<side;++i)
    tally[i]=0;

  countBox(tally,start_pos);
  //print_array(tally);

  for (int i=start_pos;i<start_pos+(3*side);i+=side)
  {
    // zero it
    for (int j=0;j<side;++j)
      row_tally[j] = 0;

    // count the row
    for (int j=i;j<i+3;++j)
      countTally(row_tally,j);
    //print_array(row_tally);

    for (int j=0;j<side;++j)
    {
      if (row_tally[j]==tally[j] && tally[j] > 1)
      {
        char c = '1'+j;
        int current_row = i/side;
        //std::cout << j+1 << ": " << current_row << "\n";
        for (int k=1;k<3;++k)
        {
          int base = (current_row/3)*3;
          int row_t = (current_row+k)%3;
          int row = base+row_t;
          //std::cout << row << "\n";
          remove_from_row(row,c);
        }

      }
    }
  }
  delete[] tally;
  delete[] row_tally;
}

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

void sudoku::countTally(int * tally, int x)
{
  char a='1';
  for (int i=0;i<side;++i)
  {
    if (options[x].count(a)==1)
    {
      ++tally[i];
    }
    ++a;
  }
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
    //not working
    //box();
    push_options();

    filled();
    if (prev == num_filled)
      return;
    prev=num_filled;
  }
}

void sudoku::remove_from_row(int x, char c)
{
  int start_pos=x*side;
  for (int i=start_pos;i<start_pos+side;++i)
    options[i].erase(c);
}

void sudoku::print_options(int x)
{
  std::set<char>::iterator it=options[x].begin();
  while (it!=options[x].end())
  {
    std::cout << *it << " ";
    ++it;
  }
  std::cout << "\n";
}

void sudoku::print_array(int * array)
{
  for (int i=0;i<side;++i)
    std::cout << array[i] << " ";
  std::cout << "\n";

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
