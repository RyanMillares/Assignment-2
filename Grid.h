#include <iostream>
using namespace std;

class Grid{
  public:
    Grid(); //default constructor
    Grid(int horz, int vert); //overloaded constructor
    ~Grid(); //deconstructor

    void scan(int mode, string fileName); //scan mainGrid and make scanGrid as next stage
    void output(); //print the current state of mainGrid to console
    void play(bool check, string file, double prob); //most the game functions
    void edit(); //edit state of individual points
    void copyTo();//copy every element of scanGrid onto mainGrid
    void checkFile(string file); //read in a file name and scan it onto the mainGrid
    void makePreset(); //allow for use of preset
    void randFill(double prob); //fill grid according to probability
    void printGrid(string fileName); //output grid to textfile fileName
    //presets
    void factory();
    void glider();
    void ship();


  private:
    //array of boolean values takes up less space because
    //it is binary data type, so less memory leak
      bool **mainGrid; //the main grid that is outputted
      bool **scanGrid; //the grid that is formed from the main grid and
                      //represents the next cycle in the simulation.

      int mode;
      int x_length; //horizontal length of grid
      int y_length; //vertical length of grids
      //these values get switched somewhere along the way



};
