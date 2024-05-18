#include <SDL2/SDL.h>
#include <stdio.h>
#include "chip8.h"

const int SCREEN_WIDTH = 64 * 10;
const int SCREEN_HEIGHT = 32 * 10;

//Background and foreground colors
const int br = 0xFF;
const int bg = 0xFF;
const int bb = 0xFF;
const int fr = 0x00;
const int fg = 0x00;
const int fb = 0x00;

chip8 myChip8;

int main( int argc, char* args[] ){

	//The rendering window
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if(window == NULL){
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		}
		else{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, br, bg, bb));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//init chip 8 and load game
			myChip8.initialize();
			myChip8.loadGame("pong2.c8");

			//initialize timer
			SDL_TimerID delayTimer = SDL_AddTimer(16, &chip8::decrementTimer, &myChip8);

			SDL_Event e;
			bool quit = false;
			while(!quit){

				//emulate cycle
				myChip8.emulateCycle();

				//draw if draw flag
				if(myChip8.drawFlag){
					myChip8.drawFlag = false;
					int x = 0;
					int y = 0;
					for(int i = 0; i < 2048; i++){

						//printf("%d ", myChip8.gfx[i]);

						if(myChip8.gfx[i]){
							SDL_Rect rect = {x * 10, y * 10, 10, 10};
							SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, fr, fg, fb));
						}
						else{
							SDL_Rect rect = {x * 10, y * 10, 10, 10};
							SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, br, bg, bb));
						}
						x++;
						if(x >= 64){
							//printf("%d \n", y);
							x = 0;
							y++;
						}
						
					}
				}
				
				SDL_UpdateWindowSurface(window);



				//Handles events on queue
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN){
						switch (e.key.keysym.sym)
						{
						case SDLK_p:
							printf("P pressed\n");
							break;
						case SDLK_1:
							myChip8.key[1] = 1;
							break;
						case SDLK_2:
							myChip8.key[2] = 1;
							break;
						case SDLK_3:
							myChip8.key[3] = 1;
							break;
						case SDLK_4:
							myChip8.key[0xC] = 1;
							break;
						case SDLK_q:
							myChip8.key[4] = 1;
							break;
						case SDLK_w:
							myChip8.key[5] = 1;
							break;
						case SDLK_e:
							myChip8.key[6] = 1;
							break;
						case SDLK_r:
							myChip8.key[0xD] = 1;
							break;
						case SDLK_a:
							myChip8.key[7] = 1;
							break;
						case SDLK_s:
							myChip8.key[8] = 1;
							break;
						case SDLK_d:
							myChip8.key[9] = 1;
							break;
						case SDLK_f:
							myChip8.key[0xF] = 1;
							break;
						case SDLK_z:
							myChip8.key[0xA] = 1;
							break;
						case SDLK_x:
							myChip8.key[0] = 1;
							break;
						case SDLK_c:
							myChip8.key[0xB] = 1;
							break;
						case SDLK_v:
							myChip8.key[0xF] = 1;
							break;
						}
						
					}
					else if(e.type == SDL_KEYUP){
						switch (e.key.keysym.sym)
						{
						case SDLK_1:
							myChip8.key[1] = 0;
							break;
						case SDLK_2:
							myChip8.key[2] = 0;
							break;
						case SDLK_3:
							myChip8.key[3] = 0;
							break;
						case SDLK_4:
							myChip8.key[0xC] = 0;
							break;
						case SDLK_q:
							myChip8.key[4] = 0;
							break;
						case SDLK_w:
							myChip8.key[5] = 0;
							break;
						case SDLK_e:
							myChip8.key[6] = 0;
							break;
						case SDLK_r:
							myChip8.key[0xD] = 0;
							break;
						case SDLK_a:
							myChip8.key[7] = 0;
							break;
						case SDLK_s:
							myChip8.key[8] = 0;
							break;
						case SDLK_d:
							myChip8.key[9] = 0;
							break;
						case SDLK_f:
							myChip8.key[0xE] = 0;
							break;
						case SDLK_z:
							myChip8.key[0xA] = 0;
							break;
						case SDLK_x:
							myChip8.key[0] = 0;
							break;
						case SDLK_c:
							myChip8.key[0xB] = 0;
							break;
						case SDLK_v:
							myChip8.key[0xF] = 0;
							break;
						}
					}
				}
				SDL_Delay(1);
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	printf("run\n");
	return 0;
}
