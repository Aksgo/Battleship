#pragma once
#include "Ship.h"
#include "ShipStates.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Field{
    int rows;
    int cols;
    int numberOfShips;
    int activeShips;
    vector<vector<string>> status;
    vector<vector<int>> position;
    vector<Ship> Ships;
    states state;
public:
    string name;
    Field(int nRows, int nCols,string pname);
    
    bool placeShip(string &cell, int angle, int shipid);
        
    int getShipCount();
    void getShip(int shipid);
    void showStatus();
    void showPosition();
    bool killCell(string &cell);
    
    bool lost();
    template <typename T>
    void displayField(vector<vector<T>> grid){
        cout<<"Grid of "<<name<<endl<<"----------\n";
        cout<<"  | ";
        for(int i = 0; i<rows; i++){
            cout<<i<<" | ";
        }
        cout<<endl;
        cout<<"   ";
        for(int j=0; j<cols; j++){
            cout<<"====";
        }
        cout<<endl;
        for(int i = 0; i<rows; i++){
            cout<<i<<"|| ";
            for(int j = 0; j<cols; j++){
                cout<<grid[i][j]<<" | ";
            }
            cout<<endl;
            cout<<"   ";
            for(int j=0; j<cols; j++){
                cout<<"----";
            }
            cout<<endl;
        }
    }
}; 
