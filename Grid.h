#include <iostream>
using namespace std;

class Grid{
  public:
    Grid(); //default constructor
    Grid(int length);
    Grid(int horz, int vert); //overloaded constructor
    ~Grid(); //deconstructor

    void scan(int mode);
    void output();
    void play(bool check, string file, double prob);
    void edit();
    void copyTo();
    void checkFile(string file);
    void makePreset();
    void randFill(double prob);
    //presets
    void factory();
    void glider();
    void ship();


  private:

      bool **mainGrid;
      bool **scanGrid;

      int mode;
      int x_length;
      int y_length;



};
