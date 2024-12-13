#include <SDL2/SDL.h>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int FPS = 25;
constexpr int MS_PER_FRAME = 1000 / FPS;

bool right_pressed = false;
bool left_pressed = false;
bool up_pressed = false;
bool down_pressed = false;

// declaration of functions
//
void logic(SDL_Surface* screen_surface);

namespace {

  SDL_Window* window = NULL;

  uint32_t make_color(uint8_t r, uint8_t g, uint8_t b) {
    return r << 16 | g << 8 | b;
  }

  void finish() {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
  }
}


int main (int argc, char ** args) {

  if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ){
    return 1;
  }
 
  SDL_Surface* screen_surface = NULL;
  
  window = SDL_CreateWindow("Spaceship",SDL_WINDOWPOS_UNDEFINED, 
			    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
			    SDL_WINDOW_SHOWN);
    
  if (window == NULL) {
    return 1;
  }

  screen_surface = SDL_GetWindowSurface(window);
  uint64_t next_frame_ms = SDL_GetTicks() + MS_PER_FRAME;

  while(1) {

    logic(screen_surface);
    SDL_LockSurface(screen_surface);
    SDL_UnlockSurface(screen_surface);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(next_frame_ms - SDL_GetTicks());
    next_frame_ms += MS_PER_FRAME;
  }
};

void logic(SDL_Surface* screen_surface){

  SDL_Event event;

   while( SDL_PollEvent( &event ) ){
    switch( event.type ){
      case SDL_KEYDOWN:
	  switch (event.key.keysym.scancode) {

      // system keys

	    case SDL_SCANCODE_ESCAPE:
	      finish();
	      break;

      // moving keys

      case SDL_SCANCODE_W:
        up_pressed = true;
        break;
      case SDL_SCANCODE_A:
        left_pressed = true;
        break;
      case SDL_SCANCODE_D:
        right_pressed = true;
        break;
      case SDL_SCANCODE_S:
        down_pressed = true;
        break;
      default:
        break;
    }

    break;

      case SDL_KEYUP:
        switch (event.key.keysym.scancode) {
          case SDL_SCANCODE_A:
            left_pressed = false;
            break;
          case SDL_SCANCODE_D:
            right_pressed = false;
            break;
          case SDL_SCANCODE_W:
            up_pressed = false;
            break;
          case SDL_SCANCODE_S:
            down_pressed = false;
            break;
          default:
            break;
        } 
      break;
    }
  }
}
