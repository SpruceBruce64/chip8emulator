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
	if(SDL_Init(SDL_INIT_VIDEO) < 0 )
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

						}
					}
				}
				//wait 1 Hz
				SDL_Delay(16);
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
