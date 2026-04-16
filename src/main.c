#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constants.h"


int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* sans_24 = NULL;
TTF_Font* sans_16 = NULL;
SDL_Surface* surfaceMessage = NULL;
SDL_Texture* message = NULL;


int last_frame_time = 0;
int snake_size = 5;
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
      "Hello", 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      0
      // SDL_WINDOW_ALLOW_HIGHDPI
      // SDL_WINDOW_BORDERLESS
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
  int random = rand() % max;

  return random;
}

void print_title(char* text, TTF_Font* font) {

  SDL_Color font_color = {(int)0xFF, (int)0xFF, (int)0xFF};
  surfaceMessage = TTF_RenderText_Solid(font, text, font_color);
  message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_Rect message_rect; //create a rect
  message_rect.x = PADDING;  //controls the rect's x coordinate 
  message_rect.y = PADDING; // controls the rect's y coordinte
  message_rect.w = WINDOW_HEIGHT * 0.5; // controls the width of the rect
  message_rect.h = HEADER - surfaceMessage->h * 0.5; // controls the height of the rect
                        
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Make window bg black.
  // SDL_RenderClear(renderer); // Paint screen black.
  SDL_RenderCopy(renderer, message, NULL, &message_rect); // Add text to render queue.
  // SDL_RenderPresent(renderer); // Render everything that's on the queue.
  // SDL_RenderCopy(renderer, message, NULL, &message_rect);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);


}

void setup(void) {

  // Resets thre 'randomness' when starting the app
  srand(time(NULL));

  direction = int_random(4);

  ball.x = int_random(GAME_WIDTH);
  ball.y = int_random(GAME_HEIGHT);
  ball.width = 15;
  ball.height = 15;


  if (TTF_Init() == -1) {
    fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
    return;
  }

  sans_24 = TTF_OpenFont("./OpenSans.ttf", 24);
  if (!sans_24) {
    fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
    return;
  }

  sans_16 = TTF_OpenFont("./OpenSans.ttf", 16);
  if (!sans_16) {
    fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
    return;
  }
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
      return;
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

  if (ball.x > GAME_WIDTH_END   ) ball.x = GAME_WIDTH_START;
  if (ball.x < GAME_WIDTH_START ) ball.x = GAME_WIDTH_END;
  if (ball.y > GAME_HEIGHT_END  ) ball.y = GAME_HEIGHT_START;
  if (ball.y < GAME_HEIGHT_START) ball.y = GAME_HEIGHT_END;
}


void render(void) {
  SDL_SetRenderDrawColor(renderer,  (int)0xC4, (int)0xC4, (int)0xC4, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect bg_rect = {
    GAME_WIDTH_START, 
    GAME_HEIGHT_START,
    GAME_WIDTH,
    GAME_HEIGHT
  };
  SDL_RenderFillRect(renderer, &bg_rect);

  // Draw a rectangle
  SDL_SetRenderDrawColor(renderer,  255, 255, 255, 255);
  SDL_Rect ball_rect = {
    (int)ball.x, 
    (int)ball.y, 
    (int)ball.width, 
    (int)ball.height
  };

  SDL_RenderFillRect(renderer, &ball_rect);
  print_title("Super Snake", sans_24);
  // TODO: Here's where we can start
  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  TTF_CloseFont(sans_24); 
  TTF_CloseFont(sans_16);
  TTF_Quit();
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
