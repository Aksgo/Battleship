#include "../include/Ship.h"
#include <iostream>

Ship::Ship(int  new_sz, int new_shipid){
        length = new_sz;
        shipid = new_shipid;
        shipHealth = length;
}

void Ship::showDetails(){
	using namespace std;
        cout<<"length : "<<length;
        cout<<", shipid : "<<shipid;
        if (angle == 2) 
        {
            cout << ", shape: L3";
        }
        cout<<endl;
}
int Ship::getX(){
        return corX;
}
int Ship::getY(){
        return corY;
}
int Ship::getLength(){
	return length;
}
int Ship::getAngle(){
        return angle;
}
void Ship::setPosition(int x, int y, int shipAngle){
        corX = x;
        corY = y;
        angle = shipAngle;
}

bool Ship::attack(){
        shipHealth--;
        return shipHealth<=0;
}

