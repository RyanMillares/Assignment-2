#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Grid.h"
using namespace std;

int main(int argc, char **argv){

  //REMINDER "5" is placeholder for cin
  string confirm;
  string file = " ";
  double x, y;
  bool check = true;

  cout << "Would you like to provide a map file? Enter 'y' if yes." << endl;
  cin >> confirm;
  if(confirm == "y"){

    cout << "Please provide the textfile for reading: ";
    cin >> file;
    string currentLine;
    int numLines = 0;
    ifstream dim(file);
    if(dim){
      getline(dim, currentLine);
      x = stoi(currentLine); //read in first line as length
      getline(dim, currentLine);
      y = stoi(currentLine); //read in second line as height
      if((x < 1)&&(y < 1)){
        dim.close();
        cout << "sorry you can't have negative or zero as a grid dimension." << endl;
        exit(0);
      }
      while(getline(dim, currentLine)){
        numLines++;
        cout << (currentLine.length() - 1) << endl;
        if((currentLine.length() - 1) != x){
          //error handling for file grid length inconsistency
          cout << "one of your lines is not as long as you said it was..." << endl;
          cout << "fix your file then try again lol" << endl;

          dim.close();
          exit(0);
        }
      }
      dim.close();
      if(numLines != y){ //error checking for file grid height
        cout << "your grid is not as tall as you said it was..." << endl;
        cout << "fix your file then try again lol" << endl;
        exit(0);
      }

      Grid myGame(y, x); //first one is y, second is x
      myGame.play(true, file, 0);
    }

    else{
      cout << "Thats not a file silly." << endl;
      cout << "Please provide the x and y dimensions." << endl;

      while(check){
        cout << "x: ";
        cin >> x;
        if(cin.fail()){
          cout << "Please only put int value." << endl;
          cin.clear();
          cin.ignore(10000,'\n');
        }
        else{
          check = false;
        }
      }
      check = true;
      while(check){
        cout << "y: ";
        cin >> y;
        if(cin.fail()){
          cout << "Please only put int value." << endl;
          cin.clear();
          cin.ignore(10000,'\n');
        }
        else{
          check = false;
        }
      }
      check = true;
      if((x <= 0) || (y <= 0)){ //only positive numbers for grid dimensions
        cout << "please, only put in positive numbers for dimensions" << endl;
        exit(0);
      }
    }
  }
    else{ //no want file
      cout << "Please provide the x and y dimensions." << endl;
      while(check){
        cout << "x: ";
        cin >> x;
        if(cin.fail()){
          cout << "Please only put int value." << endl;
          cin.clear();
          cin.ignore(10000,'\n');
        }
        else{
          check = false;
        }
      }
      check = true;
      while(check){
        cout << "y: ";
        cin >> y;
        if(cin.fail()){
          cout << "Please only put int value." << endl;
          cin.clear();
          cin.ignore(10000,'\n');
        }
        else{
          check = false;
        }
      }
      check = true;
      if((x <= 0) || (y <= 0)){
        cout << "please, only put in positive numbers for dimensions" << endl;
        exit(0);
      }
      Grid myGame(y, x); //first one is y, second is x
      cout << "Would you like to manually add points or have a random configuration?" << endl;
      cout << "'r' - random; 'm' - manual input" << endl;
      cin >> confirm;
      if(confirm == "r"){
        cout << "Provide a decimal probability for a cell to be alive." << endl;
        cin >> x;
        if(x > 1){
          x = 1;
        } else if(x < 0){
          x = 0;
        }
        myGame.play(false, file, x);

      }
      else{
        myGame.play(false, file, 0);

      }
    }
//https://stackoverflow.com/questions/877523/error-request-for-member-in-which-is-of-non-class-type
  //Grid myGame(x,y);
  return 0;
}
