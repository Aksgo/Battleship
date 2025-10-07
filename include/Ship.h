#pragma once
class Ship{
public:
	Ship(int new_sz, int new_shipid);
	void showDetails();
	int getX();
	int getY();
	int getLength();
	int getAngle();
	void setPosition(int x, int y, int shipAngle);
	bool attack();
private:
	int shipid;
	int length;
	int shipHealth;
	int angle; // 0 -> horizontal 1 -> vertical
	//top/left coordinates of ship
	int corX;
	int corY;
};
