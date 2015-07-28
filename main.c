#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>


static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;
static const int PLAYER_WIDTH = 20;
static const int PLAYER_HEIGHT = 20;
static const int POINT_WIDTH = 5;
static const int POINT_HEIGHT = 5;
static const int MOVE = 10;

typedef struct
{
	int r,g,b,a;

}colour;

SDL_Window *init(char *title, int width, int height);
SDL_Renderer *createRenderer(SDL_Window *screen);
void handleInput(SDL_Rect *player, SDL_Event e, int *end);
TTF_Font *loadFont(char *filename, int fontSize, SDL_Renderer *render);
SDL_Texture *renderScore(TTF_Font *font, SDL_Color colour, SDL_Rect *size, SDL_Renderer *render, int score);
int main(int argc, char *argv[])
{
	srand(time(NULL));
	SDL_Window *screen;
	SDL_Renderer *renderPrim;
	SDL_Event eventHandle;
	SDL_Texture *scoreDisplay;
	TTF_Font *defaultText;
	SDL_Color cPlayer = {0,255,255,255};
	SDL_Color cPoint = {255,255,0,255};
	SDL_Color cBackground = {0,0,0,255};
	SDL_Color cScore = {0,255,0,255};
	int runningGame,gotPoint, pointCount;
	runningGame = 0;
	gotPoint = 0;
	pointCount = -1;//first point spawns on player
	SDL_Rect rPlayer = {0,0,PLAYER_WIDTH, PLAYER_HEIGHT};
	SDL_Rect rPoint = {0,0,POINT_WIDTH,POINT_HEIGHT};
	SDL_Rect rScore = {0,0,0,0};
	screen = init("Piss easy snake", SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPrim = createRenderer(screen);
	defaultText = loadFont("default.ttf", 15, renderPrim);
	while(runningGame == 0)
	{
	if(gotPoint == 1)
	{
		pointCount++;
		rPoint.x = rand() % SCREEN_WIDTH;//new random position
		rPoint.y = rand() % SCREEN_HEIGHT;
		gotPoint = 0;
	}
		while(SDL_PollEvent(&eventHandle) != 0)
		{
			if(eventHandle.type == SDL_KEYDOWN)
			{
				handleInput(&rPlayer, eventHandle, &runningGame);
			
			}
			else if(eventHandle.type == SDL_QUIT)
			{
				runningGame = 1;
			
			}
		
		
		}
		SDL_DestroyTexture(scoreDisplay);//stops memory overflow
		SDL_SetRenderDrawColor(renderPrim, cBackground.r,cBackground.g,cBackground.b,cBackground.a);
		SDL_RenderClear(renderPrim);
		SDL_SetRenderDrawColor(renderPrim,cPlayer.r,cPlayer.g,cPlayer.b,cPlayer.a);
		SDL_RenderFillRect(renderPrim,&rPlayer);
		SDL_SetRenderDrawColor(renderPrim,cPoint.r,cPoint.g,cPoint.b,cPoint.a);
		SDL_RenderFillRect(renderPrim,&rPoint);
		scoreDisplay = renderScore(defaultText,cScore,&rScore, renderPrim, pointCount);
		SDL_RenderCopy(renderPrim, scoreDisplay, NULL, &rScore); 
		SDL_RenderPresent(renderPrim);
		
		gotPoint = SDL_HasIntersection(&rPlayer,&rPoint);
	}
	SDL_DestroyRenderer(renderPrim);
	SDL_DestroyWindow(screen);
	TTF_CloseFont(defaultText);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

SDL_Window *init(char *title, int width, int height)
{
	
	
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "SDL failed to initialise, %s", SDL_GetError());
		return NULL;
	
	}
	if(TTF_Init() == -1)
	{
		fprintf(stderr, "SDL_TTF failed to initialise : %s\n", TTF_GetError());
		return NULL;
	}
	SDL_Window *temp = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(!temp)
	{
		fprintf(stderr, "Window failed to initialise : %s", SDL_GetError());
		return NULL;
	
	}
	return temp;

}
SDL_Renderer *createRenderer(SDL_Window *screen)
{
	SDL_Renderer *temp = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if(!temp)
	{
		fprintf(stderr, "Renderer failed to initialise : %s", SDL_GetError());
		return NULL;
	
	}
	SDL_SetRenderDrawColor(temp,0,0,0,255);
	return temp;

}
void handleInput(SDL_Rect *player, SDL_Event e, int *end)
{
				switch(e.key.keysym.sym)
				{
					case SDLK_DOWN:
						if(player->y < SCREEN_HEIGHT - PLAYER_HEIGHT)
						{
							player->y += MOVE;
						
						}
					break;
					case SDLK_UP:
						if(player->y > 0)
						{
							player->y -= MOVE;
						
						}
					break;
					case SDLK_LEFT:
						if(player->x > 0)
						{
							player->x -= MOVE;
						
						}
					break;
					case SDLK_RIGHT:
						if(player->x < SCREEN_WIDTH - PLAYER_WIDTH)
						{
							player->x += MOVE;
						
						}
						break;
					case SDLK_ESCAPE:
						*end = 1;
						break;
					break;
				
				}



}
TTF_Font *loadFont(char *filename, int fontSize, SDL_Renderer *render)
{
	TTF_Font *temp = TTF_OpenFont(filename, fontSize);
	if(!temp)
	{
		fprintf(stderr, "Font failed to initialise : %s", TTF_GetError());
		return NULL;
	
	}
	return temp;
}

SDL_Texture *renderScore(TTF_Font *font, SDL_Color colour, SDL_Rect *size, SDL_Renderer *render, int score)
{
	SDL_Surface *temp;
	SDL_Texture *tempTex;
	char text[100];
	sprintf(text,"Score : %d", score);//makes a decent string
	temp = TTF_RenderText_Solid(font,text,colour);//this creates a surface from the text
	if(!temp)
	{
		fprintf(stderr, "Text unable to be created : %s\n", TTF_GetError());
		return NULL;
	
	}
	tempTex = SDL_CreateTextureFromSurface(render,temp);//makes a texture
	if(!tempTex)
	{
	
		fprintf(stderr, "Texture failed to create : %s", SDL_GetError());
		return NULL;
	}
	size->w = temp->w;
	size->h = temp->h;
	SDL_FreeSurface(temp);
	return tempTex;
}