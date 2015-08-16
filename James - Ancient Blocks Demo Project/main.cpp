//Filename: main.cpp
//Author: Quinn James
//Description: Loading, unloading, and main game loop

// CAVE OF ANCIENT BLOCKS

// Graphics credits:
// Player sprite: "Planet Cute" art by Daniel Cook (Lostgarden.com), released under CC Attribution 3.0
// Map tileset by Kenney.nl, released under CC0

// Music credits:
// "Hydrogen" by Quinn James



#include <stdio.h>
#include <list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "mappy_A5.h"

#include "playerdefs.h"
#include "Player_class.h"
#include "Block_class.h"



// enum for the keys - values in playerdefs.h
// player class gets it via EXTERN
bool keys[] = { false, false, false, false };

// these functions are most easily implemented in main as they involve different object instances interacting
// ==POTENTIAL ISSUE -- check whether this can be moved
bool spriteCoordCollide(Player*, Block*);
bool blockCoordCollide(Block*, Block*);
void debugPlayerMovement(ALLEGRO_FONT*, Player*);


int main(void)
{
	char mapFilename[] = "Map2-multilayer4.FMP";

	int collided(int, int);
	bool win = false;
	bool redrawRequired = true;
	// ==POTENTIAL ISSUE -- the meaning of this variable is opaque
	short int user3 = 0;	//Value returned from checkCollision function
	int mapxoff, mapyoff;
	bool done = false;
	bool render = false;

	// pointers to event sources and queue
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;

	ALLEGRO_SAMPLE *electricSound = NULL;

	// sample ID is not declared as a pointer
	ALLEGRO_SAMPLE_ID elSoundID;
	
	//program init
	if (!al_init())										//initialize Allegro
	{
		fprintf(stderr, "Failed to initialize allegro!\n");
		return -1;
	}


	//Install sound "equipment"!
	// ==POTENTIAL ISSUE -- Opaque error codes
	if (!al_install_audio()) return -9;
	if (!al_init_acodec_addon()) return -9;
	//Reserve one voice.  You need as many of these as you have
	//different sounds.
	if (!al_reserve_samples(1)) return -9;

	al_init_font_addon();
	al_init_ttf_addon();
	// load font
	ALLEGRO_FONT *arial18 = al_load_font("arial.ttf", 18, 0);
	if (!arial18)
	{
		return -3;
	}

	//Initialize keyboard input
	if (!al_install_keyboard())
	{
		fprintf(stderr, "Failed to initialize keyboard!\n");
		return -1;
	}
	//Initialize timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "Failed to initialize timer!\n");
		return -1;
	}
	//Initialize display
	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
	{
		fprintf(stderr, "Failed to initialize display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//addon image init
	al_init_image_addon();

	// initialize player sprite
	Player *player1 = new Player;
	// initialize blocks
	// The size of blockArray is now defined in playerdefs.h.
	// This makes it much easier to add new blocks because now all of the for loops that need to iterate over all
	// the blocks (for drawing and collision detection) refer to BLOCK_NUM, not a fixed integer.
	
	// ==POTENTIAL ISSUE - This looks ridiculous. Oh god this looks ridiculous.
	// std::list might help. Or some kind of operator overloading.

	
	//std::list<Block> blockList;


	Block *blockArray[BLOCK_NUM];
	blockArray[0] = new Block(8, 5);
	blockArray[1] = new Block(9, 5);
	blockArray[2] = new Block(9, 6); 
	blockArray[3] = new Block(10, 6);
	blockArray[4] = new Block(8, 7);
	blockArray[5] = new Block(9, 7);
	blockArray[6] = new Block(16, 3);
	blockArray[7] = new Block(17, 2);
	blockArray[8] = new Block(31, 6);
	blockArray[9] = new Block(27, 9);
	blockArray[10] = new Block(26, 9);
	blockArray[11] = new Block(33, 9);
	blockArray[12] = new Block(34, 10);
	blockArray[13] = new Block(34, 7);
	blockArray[14] = new Block(28, 8);

	// load map
	if (MapLoad(mapFilename, 1) != 0)
	{
		fprintf(stderr, "Failed to load the Mappy map.\n");
		return -7;
	}

	//Load the .ogg file
	electricSound = al_load_sample("Hydrogen.ogg");
	if (!electricSound) return -9;


	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "Failed to create event queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());



	

	// These are basically standard sound parameters. Gain is volume; gain and speed have 1 as normal.
	// Pan is -1 to 1, 0 is normal.
	float gain = 1.0, pan = 0.0, speed = 1.0;
	al_play_sample(electricSound, gain, pan, speed, ALLEGRO_PLAYMODE_LOOP, &elSoundID);

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT evt;
		al_wait_for_event(event_queue, &evt);

		switch (evt.type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
		
			switch(evt.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			}
			break;

			case ALLEGRO_EVENT_KEY_UP:
		
			switch(evt.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			}
			break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
		
				done = true;
				break;
		
			case ALLEGRO_EVENT_TIMER:

			// BEGIN TIMER CASE
			// ==POTENTIAL ISSUE -- case numbers are opaque. Replace with something readable.
			// Another enum could work.
				// Try to move player
				player1->updatePlayer();
				// Check collision with background map
				if ((user3 = player1->checkCoordMapCollision()))
				{
					switch (user3)
					{
						//Case 2:	Player can't pass. Reset to prev position.
						case 2:		
						
							  player1->resetPosition(); // resests coords and x/y and sets counter to 0
							break;
						case 3: // arguably this should be controlled with a different user variable, but for now, this is more concise
							win = true;
							break;

					}//END SWITCH
				}//endif

				// Check collision with blocks -- this is the most important collision in the game
				int l = -1;
				for (int k = 0; k < BLOCK_NUM; k++)
				{
					if (spriteCoordCollide(player1, blockArray[k]))
					{
						player1->resetPosition(); // resests coords and x/y and sets counter to 0
						l = k;
						blockArray[l]->pushBlock(player1->getFacing());
						k = BLOCK_NUM;
					} //endif
				} //endfor

				// Check collision between blocks and background map (no case 3, a block can't win the game)
				if (l >= 0)
				{

					if ((user3 = blockArray[l]->checkCollision()))
					{
						switch (user3)
						{
							//Case 2:	Block can't pass. Reset to prev position.

						case 2:

							blockArray[l]->resetPosition(); // resests coords and x/y
							break;

						}//END SWITCH
					} //endif
					// Check collision between blocks and other blocks
					for (int m = 0; m < BLOCK_NUM; m++)
					{
						if (m != l && user3 != 2)
						{
							if (blockCoordCollide(blockArray[l], blockArray[m]))
							{
								blockArray[l]->resetPosition();
								m = BLOCK_NUM;
							} // endif
						} // endif
					} // endfor

				} //endif


			// 172 is 36 + 72 + 72, or 2 1/2 tiles to the right of the center of the player.
			mapxoff = player1->getX() + player1->getWidth() / 2 - WIDTH + 36 + 72 + 72;
			mapyoff = player1->getY() + player1->getHeight() / 2 - HEIGHT + 36 + 72 + 72;

			//Avoid moving beyond the map edge
			if (mapxoff < 0) mapxoff = 0;
			if (mapxoff >(mapwidth * mapblockwidth - WIDTH))
				mapxoff = mapwidth * mapblockwidth - WIDTH;
			if (mapyoff < 0)
				mapyoff = 0;
			if (mapyoff >(mapheight * mapblockheight - HEIGHT))
				mapyoff = mapheight * mapblockheight - HEIGHT;

			redrawRequired = true;
			break;
			// END TIMER CASE

		} //end switch

		if(redrawRequired && al_is_event_queue_empty(event_queue))
		{
			redrawRequired = false;

			MapDrawBG(mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);
			MapDrawFG(mapxoff, mapyoff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);

			player1->drawPlayer(mapxoff, mapyoff);
			
			for (int k = 0; k < BLOCK_NUM; k++)
				blockArray[k]->drawBlock(mapxoff, mapyoff);

			// ==POTENTIAL ISSUE -- debug code in comments instead of debug functions.

			//debugPlayerMovement(arial18, player1);

			//for (int q = 0; q < BLOCK_NUM; q++)
				// This text can also be uncommented if we need more info to debug movement.
				//al_draw_textf(arial18, al_map_rgb(255, 255, 255), 500, (300 + (q * 25)), ALLEGRO_ALIGN_RIGHT, "Coords (%i %i)", blockArray[q]->getCX(), blockArray[q]->getCY());

			if (win)
				al_draw_textf(arial18, al_map_rgb(0, 0, 255), 700, 100, ALLEGRO_ALIGN_RIGHT, "YOU WIN!"); // You sure do.
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

		}
	}
	// delete player sprite
	delete player1;
	// delete blocks
	for (int z = 0; z < BLOCK_NUM; z++)
		delete blockArray[z];
	al_destroy_sample(electricSound);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_font(arial18);
	//Free memory allocated to the Mappy map
	MapFreeMem();

	return 0;
} // end main

//void addBlockToList(int x, int y, std::list<Block> *blockList)
//{
//	blockList->push_back(new Block(x, y));
//}

// mappy collide test function
int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}//END collided

bool spriteCoordCollide(Player *playerf, Block *blockf)
{
	if (playerf->getCX() == blockf->getCX() && playerf->getCY() == blockf->getCY())
		return true;
	else
		return false;
} //END spriteCoordCollide

bool blockCoordCollide(Block *blockf, Block *blockf2)
{
	if (blockf2->getCX() == blockf->getCX() && blockf2->getCY() == blockf->getCY())
		return true;
	else
		return false;
} //END blockCoordCollide

void debugPlayerMovement(ALLEGRO_FONT *arial18, Player *player1)
{
	al_draw_textf(arial18, al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_RIGHT, "Facing (%c) Movecounter (%i)", player1->getFacing(), player1->getCounter());
	al_draw_textf(arial18, al_map_rgb(255, 255, 255), 500, 200, ALLEGRO_ALIGN_RIGHT, "Coords (%i %i)", player1->getCX(), player1->getCY());
}