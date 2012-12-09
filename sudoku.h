#include<set>
class sudoku
{
  public:
    sudoku(int);
    ~sudoku();
    void read();
    void print();
    void solve();

    bool isCorrect();
    bool checkRow(int);
    bool checkCol(int);
    bool checkBox(int);
    bool isSolved();

    void pop_options();
    void push_options();
    void reduce_options();
    void hidden();
    void row_direct(int);
    void row_hidden(int);
    void column_direct(int);
    void column_hidden(int);
    void box();
    void box_direct(int);
    void box_hidden(int);
    void box_row(int);
    void box_column(int);

    void remove_from_row(int,char);
    void remove_from_col(int,char);
    void print_options(int);
    void print_array(int*);

    int filled();
    void countTally(int*,int);
    void countBox(int*,int);

  private:
    int side;
    char * grid;
    std::set<char> * options;
};
