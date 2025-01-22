#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_Texture* green_light_texture = NULL;
static SDL_FRect recs[2] = { 0 };
static Uint64 frametime = 0;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	if ( !SDL_Init(SDL_INIT_VIDEO) ) {
		return SDL_APP_FAILURE;
	}

	if ( !SDL_CreateWindowAndRenderer("Hello World", 640, 480, 0, &window, &renderer) ) {
		return SDL_APP_FAILURE;
	}
	
	SDL_SetRenderScale(renderer, 2.0f, 2.0f);	

	int x,y,n;
	unsigned char* data = stbi_load("green_light.png",&x,&y,&n,0);

	green_light_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_STATIC,
			x,
			y);
	SDL_SetTextureScaleMode(green_light_texture, SDL_SCALEMODE_NEAREST);
	SDL_UpdateTexture(green_light_texture, NULL, (void*)data, x * n);
	recs[0].w = x/2;
	recs[0].h = y;
	recs[1].x = x/2;
	recs[1].w = x/2;
	recs[1].h = y;
	stbi_image_free(data);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch ( event->type ) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		break;
		case SDL_EVENT_KEY_DOWN:
			switch ( event->key.key ) {
				case SDLK_ESCAPE:
					return SDL_APP_SUCCESS;
				break;
			}
		break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	static int frame = 0;
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, green_light_texture, &recs[frame], &recs[0]);
	SDL_RenderPresent(renderer);
	static Uint64 prev = 0;
	const Uint64 now = SDL_GetTicks();
	frametime = now - prev;
	prev = now;
	static Uint64 timer = 0;
	timer += frametime;
	if ( timer >= 1000 ) {
		timer = 0;
		frame = (frame+1)%2;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDL_DestroyTexture(green_light_texture);
}
