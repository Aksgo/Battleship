#include "../include/ShipFunc.h"
#include <iostream>

namespace ShipFunc{
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
}
