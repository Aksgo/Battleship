#include <iostream>
#include "../include/Field.h"
#include "../include/ShipFunc.h"

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
    ShipFunc::setGameShips(alice);
    alice.showPosition();
    cout<<endl;
    cout<<"Set Ships -> "<<bob.name<<" \n";
    ShipFunc::setGameShips(bob);
    bob.showPosition();

    //start the battle
    startBattle(alice,bob);
    return 0;
}
