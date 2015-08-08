//FILE: Block_class.h
//PROG: Quinn James
//PURP: Definition for block class

class Block
{
private:
	int coordx, coordy, oldcx, oldcy, movecounter, width, height, boundx, boundy;
	float x, y, oldx, oldy;
	char facing;
	ALLEGRO_BITMAP *block_img;

public:
	Block(int, int);
	~Block();
	float getX() { return x; }
	float getY() { return y; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	void drawBlock(int, int);
	int getCX() { return coordx; }
	int getCY() { return coordy; }
	void moveEast();
	void moveWest();
	void moveNorth();
	void moveSouth();
	void pushBlock(char);
	void resetPosition();
	short int checkCollision();
};

//#endif