#include <iostream>
#include "Grid.h"
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>
using namespace std;

//default square grid
Grid::Grid(){
  mainGrid = new bool*[16];
  scanGrid = new bool*[16]; //declare bool array
  //from https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
  for(int i = 0; i < 16; ++i){

    mainGrid[i] = new bool[16];
    scanGrid[i] = new bool[16]; //create dynamic 2d array of bools
  }
  for(int i = 0; i < 16; ++i){
    for(int j = 0; j < 16; ++j){


      mainGrid[i][j] = false;
    }
  }
  x_length = 16;
  y_length = 16;
}

//overloaded rectangular grid
Grid::Grid(int vert, int horz){
  mainGrid = new bool*[vert];
  scanGrid = new bool*[vert];
  for(int i = 0; i < vert; ++i){

    mainGrid[i] = new bool[horz];
    scanGrid[i] = new bool[horz];
  }
  for(int i = 0; i <= vert - 1; ++i){
    for(int j = 0; j <= horz - 1; ++j){

      mainGrid[i][j] = false;
    }
  }
  y_length = horz;
  x_length = vert;
}

//deconstructor
Grid::~Grid(){
  delete mainGrid;
  delete scanGrid;//delete both arrays
}

void Grid::scan(int mode, string fileName){
  int count;
  int same = 0;
  for(int i = 0; i < x_length; ++i){
    for(int j = 0; j < y_length; ++j){ //for every cell of 2d array
      count = 0;
      for(int m = -1; m < 2; ++m){
        for(int n = -1; n < 2; ++n){ //for the square of 9 cells around it
          switch(mode){
            case 1: //classic
            //as long as the neighbors exist, check their status
              if((((i+m) >= 0)&&((i+m) < x_length))&&(((j+n) >= 0)&&((j+n) < y_length))){
                if(mainGrid[i + m][j + n]){
                  count++;
                }
              }
            break;
            case 2: //doughnut
            {//if x or y go below 0 or above max, roll to other side of array
              //https://stackoverflow.com/questions/23599708/how-do-i-resolve-this-error-jump-to-case-label-crosses-initialization/23599822
              int rollx = i + m;
              int rolly = j + n;
              if(rollx < 0){
                rollx = x_length - 1;
              } else if(rollx > x_length - 1){
                rollx = 0;
              }
              if(rolly < 0){
                rolly = y_length - 1;
              } else if(rolly > y_length - 1){
                rolly = 0;
              }
              if(mainGrid[rollx][rolly]){
                count++;
              }
            }
            break;
            case 3: //mirror
            {//if x or y goes below 0 or above max, set to 0 or max respectively
              int reflectx = i + m;
              int reflecty = j + n;
              if(reflectx < 0){
                reflectx = 0;
              } else if(reflectx > x_length - 1){
                reflectx = x_length - 1;
              }
              if(reflecty < 0){
                reflecty = 0;
              } else if(reflecty > y_length - 1){
                reflecty = y_length - 1;
              }

              if(mainGrid[reflectx][reflecty]){
                count++;
              }
            }
            break;
            default:
              mode = 1;
            break;
          }
        }
      }
      if(mainGrid[i][j]){
        count--; //exclude own cell count ONLY if alive
      }
      if(count <= 1){ //if one live neighbor, cell dies

        scanGrid[i][j] = false;
      } else if(count == 2){ //if two live neighbors, cell is stable

        scanGrid[i][j] = mainGrid[i][j];
      } else if(count == 3){ //if three live neighbor, cell is alive

        scanGrid[i][j] = true;
      } else{ //if four or more live neighbor, cell dies.

        scanGrid[i][j] = false;
      }
      if(mainGrid[i][j] != scanGrid[i][j]){
        same++; //increment for ever difference that occurs 
      }

    }
  }
  if(same == 0){ //if no changes occurm grid is stable and end simulation
    cout << "Grid has stabilized." << endl;
    if(fileName != "blank"){//if confirmed, print output onto given file name
      printGrid(fileName);
    }
    exit(0);
  }
  copyTo();

}


void Grid::copyTo(){ //copies all elements of scanGrid to mainGrid
  for(int i = 0; i < x_length; ++i){
    for(int j = 0; j < y_length; ++j){
      mainGrid[i][j] = scanGrid[i][j];
    }
  }
}

void Grid::output(){

  for(int i = x_length - 1; i >= 0; --i){

    cout << i << " ";
    if(i < 10){ //y-axis
      cout << " ";
    }


    for(int j = 0; j <= y_length - 1; ++j){
      if(mainGrid[i][j]){
        cout << "X";
      }
      else{
        cout << "-";
      }
      cout << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void Grid::printGrid(string fileName){
  ofstream file;
  file.open(fileName);

  for(int i = x_length - 1; i >= 0; --i){
    //test
    file << i << " ";
    if(i < 10){
      file << " ";
    }
    //test

    for(int j = 0; j <= y_length - 1; ++j){
      if(mainGrid[i][j]){
        file << "X";
      }
      else{
        file << "-";
      }
      file << "  ";
    }
    file << endl;
  }
  file << endl;
  file.close();
}

void Grid::edit(){
  int x,y; //allows user to manually edit points (turn on/off)
  cout << "Please provide the x and y coords of the cell." << endl;
  cout << "x: ";
  cin >> x;
  if(x >= y_length){
    x = y_length - 1;
  } else if(x < 0){
    x = 0;
  }

  cout << "y: ";
  cin >> y;
  if(y >= x_length){
    y = x_length - 1;
  } else if(x < 0){
    x = 0;
  }

  if(mainGrid[y][x]){ //if alive, set dead, otherwise set alive
    mainGrid[y][x] = false;
  }
  else{
    mainGrid[y][x] = true;

  }
}
void Grid::makePreset(){ //allows for a preset of points to be set active
  char d;
  cout << "Do you want to use a preset design? (y/n)" << endl;
  cin >> d;
  if(tolower(d) == 'y'){
    char pre;
    int input;
    bool confirm;
    do{
      cout << "Enter which preset number you would like to see. Anything else will leave grid blank." << endl;
      cout << "Know that each preset has optimal/required grid settings.\n" << endl;
      cout << "1 - 'glider' : small, diaganolly moving ship, optimal in large square grids." << endl;
      cout << "Required: at least 5x5, doughnut mode best\n" << endl;
      cout << "2 - 'ship' : medium sized ship moves horizonatlly to the right, optimal in long rectangle grid." << endl;
      cout << "Required: at least 7x7 but can be as long as wanted, doughnut best mode\n" << endl;
      cout << "3 - 'factory' : large structure that shoots gliders down left" << endl;
      cout << "Required: at least a 40x40 grid of any mode." << endl;

      try{
        cin >> input;

        confirm = false;
      }
      catch(...){
        cout << "Please only put one character, not a string." << endl;
        confirm = true;
      }
    } while(confirm);

    switch(input){
      case 1:
        cout << "Selcected glider." << endl;
        if((x_length >= 5)&&(y_length >= 5)){
          glider();
        }
        else{
          cout << "Sorry, your grid does not meet these requirements." << endl;
        }

      break;
      case 2:
        cout << "Selected ship." << endl;
        if((x_length >= 7)&&(y_length >= 7)){
          ship();
        }
        else{
          cout << "Sorry, your grid does not meet these requirements." << endl;
        }

      break;
      case 3:
        cout << "Selected factory." << endl;
        if((x_length >= 40)&&(y_length >= 40)){
          factory();
        }
        else{
          cout << "Sorry, your grid does not meet these requirements." << endl;
        }

      break;
      default:
        cout << "no preset selected." << endl;
        break;
    }
  }
  output();
}

void Grid::randFill(double prob){
  double chance;
  srand(time(0));
  for(int i = 0; i < x_length; ++i){
    for(int j = 0; j < y_length; ++j){
      chance = ((double) rand()/(RAND_MAX)); //creates probability
      if(chance < prob){
        mainGrid[i][j] = true;
      }
    }
  }
  output();
  cout << "Points filled randomly according to probability" << endl;
}
//take in a file name and create instream to read values from
void Grid::checkFile(string file){
  string line;
  ifstream map(file);
  cout << "Converting file map to grid..." << endl;
  getline(map, line);
  getline(map, line);
  for(int i = x_length - 1; i >= 0; --i){
    try{
      getline(map, line);
      for(int j = 0; j < y_length; ++j){
        mainGrid[i][j] = (line[j] == 'X');
      }
    }
    catch(...){
      cout << "task failed Successfully!" << endl;
    }
  }
  cout << "Successfully converted!" << endl;
  output();
}

void Grid::play(bool check, string file, double prob){
  bool pause, print;
  string name = "blank";
  char c;
  int x,y;
  if(check){
    checkFile(file);
  }
  cout << "Welcome to Life! Please select a mode." << endl;
  cout << "Put 1 for Classic, 2 for Doughnut, 3 for Mirror" << endl;
  cout << "Any invalid choice will default to Classic mode." << endl;
  cin >> mode;
  //mode = 1; //test

  makePreset();
  if(prob != 0){
    randFill(prob);
  }

  cout << "Edit any points? (y/n)" << endl;
  cin >> c;
  if(tolower(c) == 'y'){
    while(true){ //edit loop
      edit();
      output(); //allow user to edit points
      cout << "Edit more points? (y/n)" << endl;
      cin >> c;
      if(tolower(c) != 'y'){
        break;
        c = 'n';
      }
    }
  }
  cout << "Would you like to hit a key to go through each cycle manually? (y)" << endl;
  cout << "Otherwise the simulation will automatically run until grid stabilizes or infinitely." <<  endl;
  cin >> c;
  pause = (c == 'y');
  cout << "If and when grid stabilizes, print grid onto output file? (y)" << endl;
  cin >> c;
  print = (c == 'y');
  if(print){


    cout << "Please give name of output file" << endl;
    cin >> name;

  }


  cout << "Simulation commencing.\nHit any key except q to continue through." << endl;
  cout << "Hit q at any time to end the simulation." << endl;
  while(c != 'q'){ //game loop temp
    scan(mode, name); //checks for mode and file name if applicable
    output();
    //sleep(10); for later
    if(pause){
      cin >> c; //put conditional here later

    }
  }
}

//presets
void Grid::glider(){
  mainGrid[3][3] = true;
  mainGrid[2][2] = true;
  mainGrid[3][4] = true;
  mainGrid[4][3] = true;
  mainGrid[2][4] = true;
  cout << "Points filled for glider" << endl;
}
void Grid::ship(){
  mainGrid[3][1] = true;
  mainGrid[5][4] = true;
  mainGrid[4][5] = true;
  mainGrid[3][5] = true;
  for(int i = 2; i < 6; ++i){
    mainGrid[2][i] = true;
  }
  cout << "Points filled for ship" << endl;
}
//sample
void Grid::factory(){ //makes small gliders
  mainGrid[30][1] = true;
  mainGrid[31][1] = true;
  mainGrid[30][2] = true;
  mainGrid[31][2] = true;
  mainGrid[30][3] = true;
  mainGrid[31][3] = true;
  mainGrid[31][4] = true;
  mainGrid[30][5] = true;
  mainGrid[31][5] = true;
  mainGrid[30][6] = true;
  mainGrid[32][6] = true;
  mainGrid[31][7] = true;
  mainGrid[28][10] = true;
  mainGrid[29][10] = true;
  mainGrid[33][10] = true;
  mainGrid[34][10] = true;
  mainGrid[28][11] = true;
  mainGrid[30][11] = true;
  mainGrid[32][11] = true;
  mainGrid[34][11] = true;
  for(int i = 29; i < 34; ++i){
    mainGrid[i][12] = true;
  }
  mainGrid[30][13] = true;
  mainGrid[31][13] = true;
  mainGrid[32][13] = true;
  mainGrid[31][14] = true;
  mainGrid[26][26] = true;
  mainGrid[32][26] = true;
  for(int i = 23; i < 27; ++i){
    mainGrid[31][i] = true;
  }
  for(int i = 22; i < 26; ++i){
    mainGrid[30][i] = true;
  }
  for(int i = 23; i < 27; ++i){
    mainGrid[27][i] = true;
  }
  for(int i = 22; i < 26; ++i){
    mainGrid[28][i] = true;
  }
  mainGrid[29][22] = true;
  mainGrid[29][25] = true;
  mainGrid[30][31] = true;
  mainGrid[31][31] = true;
  mainGrid[29][35] = true;
  mainGrid[28][35] = true;
  mainGrid[29][36] = true;
  mainGrid[28][36] = true;
  cout << "Points Filled for factory." << endl;
}
