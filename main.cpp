#include <SDL2/SDL.h>
#include "headers/hitbox.h"
#include "headers/point_2d.h"

// hitboxes 
const hitbox screen_hitbox(640, 480);
const hitbox ship_hitbox(60, 60);

// constants

constexpr int FPS = 25;
constexpr int MS_PER_FRAME = 1000 / FPS;

// declaration of keys' presses

bool right_pressed = false;
bool left_pressed = false;
bool up_pressed = false;
bool down_pressed = false;

// declaration of functions

void logic(SDL_Surface* screen_surface);
void blit(SDL_Surface* dst_surface, SDL_Surface* src_surface, point_2d dst_point, bool mirror);

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
			    SDL_WINDOWPOS_UNDEFINED, screen_hitbox.width, screen_hitbox.height, 
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

// blit function is for drawing

void blit(SDL_Surface* dst_surface, SDL_Surface* src_surface, point_2d dst_point, bool mirror){
  uint32_t * dst_pixels = (uint32_t *) dst_surface->pixels;
  uint32_t pitch_in_pixels = dst_surface->pitch / 4;
  uint32_t pitch_in_pixels_src = src_surface->pitch / 4;
  uint32_t * src_pixels = (uint32_t *) src_surface->pixels; 

  for(int i = 0; i < src_surface->w; i++)
  {
    for(int j = 0; j < src_surface->h; j++)
    {
      point_2d cur_point(i, j);
      point_2d cur_dst_point = cur_point + dst_point;
      cur_dst_point = dst_point + (mirror ? point_2d(src_surface->w - cur_point.x - 1, cur_point.y) : cur_point);

        if(!(cur_dst_point.x >= 0 && cur_dst_point.x < dst_surface->w && cur_dst_point.y >= 0
             && cur_dst_point.y < dst_surface->h)){
          continue;
        }

      uint32_t color_src = src_pixels[i + j * pitch_in_pixels_src];
      uint8_t r_src = color_src >> 0;
      uint8_t g_src = color_src >> 8;
      uint8_t b_src = color_src >> 16; 
      uint8_t alpha = color_src >> 24;

      uint32_t background_color = dst_pixels[cur_dst_point.x + (cur_dst_point.y) * pitch_in_pixels];
      
      uint8_t b_background = background_color >> 0;
      uint8_t g_background = background_color >> 8;
      uint8_t r_background = background_color >> 16; 

      uint8_t r = (r_src * alpha + r_background * (255 - alpha)) / 255;
      uint8_t g = (g_src * alpha + g_background * (255 - alpha)) / 255;
      uint8_t b = (b_src * alpha + b_background * (255 - alpha)) / 255;
      dst_pixels[cur_dst_point.x + (cur_dst_point.y) * pitch_in_pixels] = make_color(r, g, b);
    }
  }
}
