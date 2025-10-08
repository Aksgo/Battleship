#include "../include/Field.h"
#include "../include/ShipShape.h"
#include "../include/ShipFunc.h"

Field::Field(int nRows, int nCols,string pname){
        name = pname;
        rows = nRows;
        cols = nCols;
        status.resize(rows,vector<string>(cols,state.unharmed));
        // 0 means no ship
        position.resize(rows,vector<int>(cols,0));
        Ships = ShipFunc::createShips();
        numberOfShips = Ships.size();
        activeShips = numberOfShips -1;
}
bool Field::placeShip(string &cell, int angle, int shipid){
        if (cell.size() != 2 || !isdigit(cell[0]) || !isdigit(cell[1]) || (angle != 0 && angle != 1 && angle != 2)) 
        {
            return false;
        }
        int row = cell[0]-'0';
        int col = cell[1]-'0';
        Ship &ship = Ships[shipid];
        int len = ship.getLength();
        if(row<0 || row>=rows || col<0 || col>=cols){
            return false;
        }
        if(angle==0){ //horizontal
            int newc = col+len-1;
            if(newc>=cols){
                return false;
            }
            for(int pos = col; pos<=newc; pos++){
                if(position[row][pos]!=0){
                    return false;
                }
            }
            for(int pos = col; pos<=newc; pos++){
                position[row][pos] = shipid;
            }
        }else if(angle==1){ //vertical
            int newr = row+len-1;
            if(newr>=rows){
                return false;
            }
            for(int pos = row; pos<=newr; pos++){
                if(position[pos][col]!=0){
                    return false;
                }
            }
            for(int pos = row; pos<=newr; pos++){
                position[pos][col] = shipid;
            }
        }else if (angle == 2) { // L3 shape
            if (len != 3) return false; // L3 only for length-3 ships
            for (const auto& offset : L3Shape.offsets) {
                int r = row + offset.first;
                int c = col + offset.second;
                if (r < 0 || r >= rows || c < 0 || c >= cols || position[r][c] != 0) {
                    return false;
                }
            }
            for (const auto& offset : L3Shape.offsets) {
                int r = row + offset.first;
                int c = col + offset.second;
                position[r][c] = shipid;
            }
        }
        ship.setPosition(row, col ,angle);
        return true;   
}
int Field::getShipCount(){
        return numberOfShips;
}
void Field::getShip(int shipid){
        Ships[shipid].showDetails();
}
void Field::showStatus(){
        displayField(status);
}
void Field::showPosition(){
        displayField(position);
}
bool Field::killCell(string &cell){
        if(cell.size()!=2){return false;}
        int row = cell[0]-'0';
        int col = cell[1]-'0';
        if(row<0 || row>=rows || col<0 || col>=cols){
            return true;
        }
        if(status[row][col]!=state.unharmed){
            return true;
        }
        status[row][col] = state.killed;
        int shipid = position[row][col];
        if(shipid==0){
            return false;
        }
        status[row][col] = state.ship;
        bool destroyed = Ships[shipid].attack();
        if(destroyed){
            cout<<"\n == Ship Destroyed == \n";
            Ship &ship = Ships[shipid];
            int strow = ship.getX();
            int stcol = ship.getY();
            int len = ship.getLength();
            int angle = ship.getAngle();
            ship.showDetails();
            if(angle==0)
            {
                for(int i = stcol; i<stcol+len; i++){
                    status[strow][i] = to_string(shipid);
                }
            }else if(angle==1){
                for(int i=strow; i<strow+len;i++){
                    status[i][stcol] = to_string(shipid);
                }
            }else if (angle == 2) {
                for (const auto& offset : L3Shape.offsets) {
                    int r = strow + offset.first;
                    int c = stcol + offset.second;
                    status[r][c] = to_string(shipid);
                }
            }
            activeShips--;
        }
        return false;
}
bool Field::lost(){
        return activeShips<=0;
}
