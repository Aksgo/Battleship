#include <iostream>
#include <vector>
using namespace std;

struct states{
    string ship = "S";
    string killed = "X";
    string unharmed = ".";
};
class Ship{
private:
    int shipid;
    int length;
    int angle; // 0 -> horizontal 1 -> vertical
    //top/left coordinates of ship
    int corX;
    int corY;
public:
    Ship(int  new_sz, int new_shipid){
        length = new_sz;
        shipid = new_shipid;
    }
    void showDetails(){
        cout<<"length : "<<length;
        cout<<", shipid : "<<shipid<<endl;
    }
    int getX(){
        return corX;
    }
    int getY(){
        return corY;
    }
    int getLength(){
        return length;
    }
    void setPosition(int x, int y, int angle){
        corX = x;
        corY = y;
        angle = angle;
    }
};

vector<Ship> createShips(){
    int numberOfShips = 5;
    vector<Ship> gameShips;
    vector<int> shipSize = {0,6,4,2,2,3};
    Ship HeadShip(-1,-1);
    gameShips.push_back(HeadShip);
    for(int shipid = 1; shipid<=numberOfShips; shipid++){
        Ship shipObj(shipSize[shipid],shipid);
        gameShips.push_back(shipObj);
    }
    return gameShips;
}

class Field{
private:
    int rows;
    int cols;
    int numberOfShips;
    vector<vector<string>> status;
    vector<vector<int>> position;
    vector<Ship> Ships;
    states state;
public:
    string name;
    Field(int nRows, int nCols,string pname){
        name = pname;
        rows = nRows;
        cols = nCols;
        status.resize(rows,vector<string>(cols,state.unharmed));
        // 0 means no ship
        position.resize(rows,vector<int>(cols,0));
        Ships = createShips();
        numberOfShips = Ships.size();
    }
    bool placeShip(string &cell, int angle, int shipid){
        if(cell.size()!=2 || (angle!=0 && angle!=1)){return false;}
        int row = cell[0]-'0';
        int col = cell[1]-'0';
        Ship ship = Ships[shipid];
        int len = ship.getLength();
        if(row<0 || row>=rows || col<0 || col>=cols){
            return false;
        }
        if(angle==0){
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
        }else if(angle==1){
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
        }
        ship.setPosition(row, col ,angle);
        return true;   
    }
    int getShipCount(){
        return numberOfShips;
    }
    void getShip(int shipid){
        Ships[shipid].showDetails();
    }
    void showStatus(){
        displayField(status);
    }
    void showPosition(){
        displayField(position);
    }
    bool killCell(string &cell){
        if(cell.size()!=2){return false;}
        int row = cell[0]-'0';
        int col = cell[1]-'0';
        if(row<0 || row>=rows || col<0 || col>=cols){
            return false;
        }
        status[row][col] = state.killed;
        int shipid = position[row][col];
        if(shipid==0){
            return true;
        }
        status[row][col] = state.ship;


    }
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

void setGameShips(Field &player){
    cout<<"Enter row-column (Eg : 15) to place ships\n\n";
    int shipid = 1;
    while(shipid<player.getShipCount()){
        player.showPosition();
        cout<<"Current Ship: ";
        player.getShip(shipid);
        cout<<"\nEnter starting cell: ";
        string coordinates;
        int angle;
        cin>>coordinates;
        cout<<"Enter angle (0 = horizontal, 1 = vertical): ";
        cin>>angle;
        if(!player.placeShip(coordinates,angle,shipid)){
            cout<<"Invalid positioning (try again)\n";
        }else{
            shipid++;
        }
    }
    cout<<"Ship Setup successful for player "<<player.name<<endl;
}

void takeShot(Field &defender){
    string cell;
    cout<<"Enter the row-column to attack :";
    cin>>cell;
    int row = cell[0]
    defender.killCell(row,col);
    defender.showStatus();
}

void startBattle(Field &alice, Field &bob){
    cout<<"=========Game Start==============\n\n";
    vector<Field> player = {alice, bob};
    int pidx = 0;
    bool game = true;
    while(game){
        cout<<player[pidx].name<<" turn's\n";
        takeShot(player[pidx^1]);
        pidx = pidx^1;
    }
}
int main()
{

    //grids for both players
    Field alice(10,10,"player 1");
    Field bob(10,10,"player 2");
    cout<<"Set Ships -> "<<alice.name<<" \n";
    setGameShips(alice);
    //cout<<"\nSet Ships (Bob)\n";
    //setGameShips(bob);

    //final position after placing the battleships
    alice.showPosition();

    //start the battle
    startBattle(alice,bob);
    return 0;
}
