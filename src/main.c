#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int last_frame_time = 0;


struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;

int init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return FALSE;
  }

  window = SDL_CreateWindow(
      NULL, 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL window\n");
    return FALSE;
  }

  renderer = SDL_CreateRenderer(
    window, 
    -1, // Default display driver
    0  // No special way of rendering
  );

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return FALSE;
  }

  return TRUE;
}

void setup(void) {
  ball.x = 20;
  ball.y = 20;
  ball.width = 15;
  ball.height = 15;
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_is_running = FALSE;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = FALSE;
      break;
  }
}
void update(void) {
  // sleep until we reach the frame target time
  while(!SDL_TICKS_PASSED(SDL_GetTicks(),last_frame_time + FRAME_TARGET_TIME) );

  // Logic to keep a fixed timestamp
  last_frame_time = SDL_GetTicks();

  ball.x += 5;
  ball.y += 5;
}
void render(void) {
  SDL_SetRenderDrawColor(renderer,  0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw a rectangle
  SDL_SetRenderDrawColor(renderer,  255, 255, 255, 255);
  SDL_Rect ball_rect = {
    (int)ball.x, 
    (int)ball.y, 
    (int)ball.width, 
    (int)ball.height
  };

  SDL_RenderFillRect(renderer, &ball_rect);
  // TODO: Here's where we can start
  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char* argv[]) {
  game_is_running = init_window();

  setup();

  while(game_is_running) {
    process_input();
    update();
    render();
  }


  destroy_window();

  return 0;
}
