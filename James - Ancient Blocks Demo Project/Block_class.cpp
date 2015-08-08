//FILE: Block_class.cpp
//PROG: Quinn James
//PURP: Implementation of block class

//Include basic definitions/prototypes for mappy
//since we need access to functions in method collided
#include "mappy_A5.h"
#include "Block_class.h"
#include "playerdefs.h"

//Class needs access to the keys array declared in main.cpp
extern bool keys[];

Block::Block(int cx_i, int cy_i)
{
	coordx = oldcx = cx_i;
	coordy = oldcx = cy_i;

	x = oldx = coordx * 72.0;
	y = oldy = coordy * 72.0;
	movecounter = 0;
	width = 72;
	height = 72;
	boundx = 36;
	boundy = 36;
	block_img = al_load_bitmap("block2.png");
} // END constructor

Block::~Block()
{
	al_destroy_bitmap(block_img);
} // END destructor




// This function needs to know which direction the player is facing.
// This entire class cares not a whit about keyboard input directly.
// However, which direction the player is facing controls which way blocks move.
// updateBlock will ONLY be called from main IFF there is a collision with the player.
// Therefore, we always are going to be moving if this is getting called.
// We will get the player_facing variable via the player class getter called in main.
// The existence of a local facing variable is useful for future movement refinement. 
void Block::pushBlock(char player_facing)
{
	oldy = y;
	oldx = x;
	oldcx = coordx;
	oldcy = coordy;
	facing = player_facing;
	if (facing == 'W') { moveWest(); coordx--; }
	else if (facing == 'E') { moveEast(); coordx++; }
	else if (facing == 'N') { moveNorth(); coordy--;  }
	else if (facing == 'S') { moveSouth(); coordy++; }
} // END pushBlock

void Block::drawBlock(int mapxoff, int mapyoff)
{
	al_draw_bitmap(block_img, (x - mapxoff), (y - mapyoff + 1), 0);
} // END drawBlock
void Block::moveEast() { x += 72.0; }
void Block::moveWest() { x -= 72.0; }
void Block::moveNorth() { y -= 72.0; }
void Block::moveSouth() { y += 72.0; }

void Block::resetPosition() {
	x = oldx;
	y = oldy;
	coordx = oldcx;
	coordy = oldcy;
} // END resetPositionn

short int Block::checkCollision()
{
	short int user3 = 0;
	int collX = x + .5 * width;
	int collY = y + .5 * height;

	BLKSTR *blockdata = MapGetBlock(collX / mapblockwidth, collY / mapblockheight);
	//If any collision bits are set, assign user3 to the return variable
	if (blockdata->tl || blockdata->tr || blockdata->bl || blockdata->br)
		user3 = blockdata->user3;
	return user3;
}//END checkCollision