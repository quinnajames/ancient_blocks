//FILE: Player_class.h
//PROG: Quinn James
//PURP: Definition for player sprite class

class Player
{
	private:
		//int collisioncounter;
		int movecounter, coordx, coordy, oldcx, oldcy;
		float x, y;
		float oldx, oldy;
		int width, height;
		ALLEGRO_BITMAP *player_img;
		char facing;
		float verticalSpeed;
		int xcount, ycount;
		int boundx, boundy;

	public:
		Player();
		~Player();
		unsigned char collided(float, float);
		void updatePlayer();
		float getX() { return x; }
		float getY() { return y; }
		int getCX() { return coordx; }
		int getCY() { return coordy; }

		int getWidth() {return width;}
		int getHeight() { return height; }
		int getFacing() { return facing; }
		void drawPlayer(int, int);
		short int checkCollision();
		short int checkCoordMapCollision();
		void resetPosition();
		int getCounter() { return movecounter; }
		void moveEast();
		void moveWest();
		void moveNorth();
		void moveSouth();
		void setCounter(int sc) { movecounter = sc; }

};

//#endif