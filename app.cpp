#include <iostream>
#include <vector>
#include <cctype> 
using namespace std;

struct states{
    string ship = "S";
    string killed = "X";
    string unharmed = ".";
};

// L3 shape for 2D support
struct ShipShape
{
    string name;
    vector<pair<int, int>> offsets; 
};
const ShipShape L3Shape = {"L3", {{0,0}, {0,1}, {1,0}}};

class Ship{
private:
    int shipid;
    int length;
    int shipHealth;
    int angle; // 0 -> horizontal 1 -> vertical
    //top/left coordinates of ship
    int corX;
    int corY;
public:
    Ship(int  new_sz, int new_shipid){
        length = new_sz;
        shipid = new_shipid;
        shipHealth = length;
    }
    void showDetails()
    {
        cout<<"length : "<<length;
        cout<<", shipid : "<<shipid;
        if (angle == 2) 
        {
            cout << ", shape: L3";
        }
        cout<<endl;
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
    int getAngle(){
        return angle;
    }
    void setPosition(int x, int y, int shipAngle){
        corX = x;
        corY = y;
        angle = shipAngle;
    }
    bool attack(){
        shipHealth--;
        return shipHealth<=0;
    }
};

vector<Ship> createShips(){
    int numberOfShips = 5;
    vector<Ship> gameShips;
    vector<int> shipSize = {0,6,4,3,3,2};
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
    int activeShips;
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
        activeShips = numberOfShips -1;
    }
    bool placeShip(string &cell, int angle, int shipid){
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
    bool lost(){
        return activeShips<=0;
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
        cout<<"Enter angle (0 = horizontal, 1 = vertical, 2 = L3 shape): ";
        cin>>angle;
        if(!player.placeShip(coordinates,angle,shipid)){
            cout<<"Invalid positioning (try again)\n";
        }else{
            shipid++;
        }
    }
    cout<<"Ship Setup successful for player "<<player.name<<endl;
}

bool takeShot(Field &defender, int &pidx){
    cout<<defender.name<<" current status board\n\n";
    defender.showStatus();
    string cell;
    cout<<"Enter the row-column to attack :";
    cin>>cell;
    cout<<endl;
    if(defender.killCell(cell)){
        pidx^=1;
        cout<<"Please choose an unattacked cell\n";
        return false;
    }
    defender.showStatus();
    cout<<"Your Attack on Grid shown above:\n ------------\n";
    return defender.lost();
}

void startBattle(Field &alice, Field &bob){
    cout<<"=========Game Start=========\n\n";
    vector<Field> player = {alice, bob};
    int pidx = 0;
    bool game = true;
    while(game){
        cout<<"Press Enter to continue...";
        cin.ignore();
        cin.get();
        cout<<player[pidx].name<<" turn's\n";
        bool result = takeShot(player[pidx^1], pidx);
        if(result == true){
            cout<<"==========================\n";
            cout<<"----- All Ships Destroyed -----\n";
            cout<<(player[pidx].name)<<" wins!";
            game = false;
        }
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
    alice.showPosition();
    cout<<endl;
    cout<<"Set Ships -> "<<bob.name<<" \n";
    setGameShips(bob);
    bob.showPosition();

    //start the battle
    startBattle(alice,bob);
    return 0;
}
