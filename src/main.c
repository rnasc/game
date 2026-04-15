#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./constants.h"


int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int last_frame_time = 0;
int snake_size = 5;

#define DIRECTION_VERTICAL_UP      0
#define DIRECTION_VERTICAL_DOWN    1
#define DIRECTION_HORIZONTAL_LEFT  3
#define DIRECTION_HORIZONTAL_RIGHT 4

int direction;

struct position {
  int x;
  int y;
} position;

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

// Retunrs a ramdom number from 0 to the received number
int int_random(int max) {
  srand(time(NULL));
  int random = rand() % max;

  return random;
}

void setup(void) {
  direction = int_random(4);

  ball.x = int_random(WINDOW_WIDTH);
  ball.y = int_random(WINDOW_HEIGHT);
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
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_Q:
          game_is_running = FALSE;
          break;
        case SDL_SCANCODE_DOWN:
          direction = DIRECTION_VERTICAL_DOWN;
          break;
        case SDL_SCANCODE_UP:
          direction = DIRECTION_VERTICAL_UP;
          break;
        case SDL_SCANCODE_LEFT:
          direction = DIRECTION_HORIZONTAL_LEFT;
          break;
        case SDL_SCANCODE_RIGHT:
          direction = DIRECTION_HORIZONTAL_RIGHT;
          break;
        // case :
        default:
          break;
      }
      // if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
      //     event.key.keysym.scancode == SDL_SCANCODE_Q) {
      //   game_is_running = FALSE;
      // }
      break;
  }
}
void update(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() + last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {  
    SDL_Delay(time_to_wait);
  }
  // sleep until we reach the frame target time
  // while(!SDL_TICKS_PASSED(SDL_GetTicks(),last_frame_time + FRAME_TARGET_TIME) );

  // Logic to keep a fixed timestamp
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
  last_frame_time = SDL_GetTicks();

  int base_move = 70;

  switch (direction) {
    case DIRECTION_HORIZONTAL_RIGHT:
      ball.x += base_move * delta_time;
      break;
    case DIRECTION_HORIZONTAL_LEFT:
      ball.x -= base_move * delta_time;
      break;
    case DIRECTION_VERTICAL_UP:
      ball.y -= base_move * delta_time;
      break;
    case DIRECTION_VERTICAL_DOWN:
      ball.y += base_move * delta_time;
  }
  // ball.x += base_move * delta_time;
  // ball.y += base_move * delta_time;

  if (ball.x > WINDOW_WIDTH ) ball.x = 0;
  if (ball.x < 0            ) ball.x = WINDOW_WIDTH;
  if (ball.y > WINDOW_HEIGHT) ball.y = 0;
  if (ball.y < 0            ) ball.y = WINDOW_HEIGHT;
  // printf("ball.x: %i \t ball.y: %i\n", (int)ball.x, (int)ball.y);
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
