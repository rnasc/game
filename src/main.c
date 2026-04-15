#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

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

  renderer = SDL_Renderer(
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

void setup(void) {}
void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_is_running = FALSE;
    case :
    case :
  }
}
void update(void) {}
void renderer(void) {}

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
