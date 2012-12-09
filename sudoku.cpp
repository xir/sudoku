#include"sudoku.h"
#include<iostream>
#include<set>

sudoku::sudoku(int side)
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

void sudoku::read()
{
  delete[] grid;
  delete[] options;
  grid = new char[side*side];
  options = new std::set<char>[side*side];
  for (int i=0;i<side*side;++i)
    std::cin >> grid[i];
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
  int num_filled = filled();
  bool solved;
  if (num_filled != side*side)
    solved = false;
  else
    solved = true;
  int prev=num_filled;
  while(!solved)
  {
    reduce_options();
    hidden();
    box();
    push_options();

    num_filled = filled();
    if (prev == num_filled)
      return;
    prev=num_filled;
  }
}

bool sudoku::isCorrect()
{
  for (int i=0;i<side;++i)
    if (!(checkRow(i) && checkCol(i) && checkBox(i)))
      return false;
  return true;
}

bool sudoku::checkRow(int n)
{
  int start_pos = n*side;
  std::set<char> elements;
  int filled_elems=0;
  for (int i=start_pos;i<side;++i)
    if (grid[i] >= '1' && grid[i] <= '9')
    {
      elements.insert(grid[i]);
      ++filled_elems;
    }
  return filled_elems == elements.size();
}

bool sudoku::checkCol(int n)
{
  int start_pos = n;
  std::set<char> elements;
  int filled_elems=0;
  for (int i=start_pos;i<side*side;i+=side)
    if (grid[i] >= '1' && grid[i] <= '9')
    {
      elements.insert(grid[i]);
      ++filled_elems;
    }
  return filled_elems == elements.size();
}

bool sudoku::checkBox(int n)
{
  int start_pos=(n/3)*3*side+(n%3)*3;
  std::set<char> elements;
  int filled_elems=0;
  for (int i=start_pos;i<start_pos+(3*side);i+=side)
    for (int j=i;j<3;++j)
    {
      elements.insert(grid[j]);
      ++filled_elems;
    }
  return filled_elems == elements.size();
}

bool sudoku::isSolved()
{
  bool isFilled = (filled() == side*side);
  return (isFilled && isCorrect());
}

void sudoku::pop_options()
{
  for (int i=0;i<side*side;++i)
    if (grid[i] == '0')
    {
      char a='1';
      for (int j=0;j<side;j++)
      {
        options[i].insert(a);
        a++;
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

void sudoku::box()
{
  for (int i=0;i<side;++i)
  {
    box_row(i);
    box_column(i);
  }
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

// examine the rows within a box
// if total options of a number are the same in the box and in the row
// then number can only occur in that row, therefore it can't occur in the
// other two rows of that box, so remove them
// edits options outside of its box
void sudoku::box_row(int n)
{
  int start_pos=(n/3)*3*side+(n%3)*3;
  int * tally = new int[side];
  int * row_tally = new int[side];

  for (int i=0;i<side;++i)
    tally[i]=0;

  countBox(tally,start_pos);

  for (int i=start_pos;i<start_pos+(3*side);i+=side)
  {
    // zero it
    for (int j=0;j<side;++j)
      row_tally[j] = 0;

    // count the row
    for (int j=i;j<i+3;++j)
      countTally(row_tally,j);

    for (int j=0;j<side;++j)
    {
      if (row_tally[j]==tally[j] && tally[j] > 1)
      {
        char c = '1'+j;
        int start_pos = (i/3)*3;
        int row = i/side;
        //std::cout << current_row << ": " << j+1 << "\n";
        for (int k=3;k<side;++k)
        {
          int pos = row*side+(start_pos+k)%side;
          options[pos].erase(c);
        }
      }
    }
  }
  delete[] tally;
  delete[] row_tally;
}

void sudoku::box_column(int n)
{
  int start_pos=(n/3)*3*side+(n%3)*3;
  int * tally = new int[side];
  int * col_tally = new int[side];

  for (int i=0;i<side;++i)
    tally[i]=0;

  countBox(tally,start_pos);
  for (int i=start_pos;i<start_pos+3;++i)
  {
    // zero it
    for (int j=0;j<side;++j)
      col_tally[j] = 0;

    // count the column
    for (int j=i;j<i+3*side;j+=side)
      countTally(col_tally,j);

    for (int j=0;j<side;++j)
    {
      if (col_tally[j]==tally[j] && tally[j] > 1)
      {
        char c = '1'+j;
        int start_pos = (i/(side*3))*side*3 + i%side;
        int column = i%side;
        for (int k=3;k<side;++k)
        {
          int pos = (start_pos+k*side)%(side*side);
          options[pos].erase(c);
        }
      }
    }
  }
  delete[] tally;
  delete[] col_tally;
}

void sudoku::remove_from_row(int n, char c)
{
  int start_pos=n*side;
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

int sudoku::filled()
{
  int num_filled=0;
  for (int i=0;i<side*side;++i)
    if (grid[i]!='0')
      ++num_filled;
  return num_filled;
}

void sudoku::countTally(int * tally, int x)
{
  char a='1';
  for (int i=0;i<side;++i)
  {
    if (options[x].count(a)==1)
      ++tally[i];
    ++a;
  }
}

// puts the number of each possibility 1-9 in 0-8 of tally
void sudoku::countBox(int * tally, int start_pos)
{
  for (int i=start_pos;i<start_pos+(3*side);i+=side)
    for (int j=i;j<i+3;++j)
      countTally(tally,j);
}
