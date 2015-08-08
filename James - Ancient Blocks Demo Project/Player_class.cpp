//FILE: Player_class.cpp
//PROG: Quinn James
//PURP: Implementation of sprite class

//Include basic definitions/prototypes for mappy
//since we need access to functions in method collided
#include "mappy_A5.h"
#include "Player_class.h"
#include "playerdefs.h"


//Class needs access to the keys array declared in main.cpp
extern bool keys[];
Player::Player()
{
	coordx = oldcx = 7; // Eventually, these values could be passed in like in the block class rather than hardcoded.
	coordy = oldcy = 6; // However, unless saving and loading the game is ever implemented, there's really no need.
	x = coordx * 72.0;
	y = coordy * 72.0;
	oldx = x;
	oldy = y;
	movecounter = 0; // set to 6 when moving so character can only move to the centers of tiles. This is CRITICAL for the game to function in an orderly fashion.
	width = 72;
	height = 72; 

	facing = 'E';
	player_img = al_load_bitmap("horngirl.png");


} // END constructor

Player::~Player()
{
	al_destroy_bitmap(player_img);
} // END destructor

void Player::updatePlayer()
{


	if (movecounter > 0)
	{
		//Ignore all keyboard inputs this cycle. DO NOT change facing.
		if (facing == 'W')
		{ moveWest(); }
		else if (facing == 'E') { moveEast(); }
		else if (facing == 'N') { moveNorth(); }
		else if (facing == 'S') { moveSouth(); }
		movecounter--;

	}
	else if (movecounter == 0)
	{
		// DO change facing, and get the movecounter going.
		oldy = y;
		oldx = x;
		oldcx = coordx;
		oldcy = coordy;
		if (keys[RIGHT])
		{
			facing = 'E';
			coordx++;
			moveEast();
			movecounter = 5;
		}
	else if (keys[LEFT])
		{
			facing = 'W';
			coordx--;
			moveWest();
			movecounter = 5;
		}
	else if (keys[UP])
		{
			facing = 'N';
			coordy--;
			moveNorth();
			movecounter = 5;
		}
	else if (keys[DOWN])
		{
			facing = 'S';
			coordy++;
			moveSouth();
			movecounter = 5;
		}
	}


} // END updatePlayer

void Player::drawPlayer(int mapxoff, int mapyoff)
{
	al_draw_bitmap(player_img, (x - mapxoff), (y - mapyoff + 1), 0);
} // END drawPlayer

short int Player::checkCollision()
{
	short int user3 = 0;
	int collX = x + .5 * width;
	int collY = y + .5 * height;

	BLKSTR *blockdata = MapGetBlock(collX / mapblockwidth, collY / mapblockheight);
	//If any collision bits are set, assign user3 to the return variable
	if (blockdata->tl || blockdata->tr || blockdata->bl || blockdata->br)
		user3 = blockdata->user3;
	return user3;
} // END checkCollision

short int Player::checkCoordMapCollision()
{
	short int cuser3 = 0;
	int ccollX = coordx * 72.0; // destination coords - get changed when movecounter starts in updatePlayer()
	int ccollY = coordy * 72.0;

	BLKSTR *blockdata = MapGetBlock(ccollX / mapblockwidth, ccollY / mapblockheight);
	//If any collision bits are set, assign user3 to the return variable
	if (blockdata->tl || blockdata->tr || blockdata->bl || blockdata->br)
		cuser3 = blockdata->user3;
	return cuser3;
} // END checkCollision

void Player::moveEast() { x += 72.0 / 6; }
void Player::moveWest() { x -= 72.0 / 6; }
void Player::moveNorth() { y -= 72.0 / 6; }
void Player::moveSouth() { y += 72.0 / 6; }

void Player::resetPosition()
{
 x = oldx;
 y = oldy;
 coordx = oldcx;
 coordy = oldcy;
 movecounter = 0;
} // END resetPosition