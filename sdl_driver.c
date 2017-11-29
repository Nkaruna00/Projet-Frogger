#include "game.h"
#include "driver.h"
#include <time.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <SDL.h>
#include <SDL_image.h>

static int init(const Game* game);
static void start(void(*callback)(void*));
static int get_move(void);
static void draw_bg(void);
static void draw_entity(int ent_id);
static void update(void);

 
Driver sdl_driver = {
    .game = NULL,
    .init = init,
    .start = start,
    .get_move = get_move,
    .draw_bg = draw_bg,
    .draw_entity = draw_entity,
    .update = update
};

#define GAME (sdl_driver.game)

static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture* tiles[NCell];
static const char* tiles_files[] = { "files/bg/grass.png", "files/bg/brique.png","files/bg/brique.png", "files/bg/water.png","files/bg/street.png",
"files/sprite/taxi.png","files/sprite/mini.png",
"files/sprite/police.png","files/sprite/ambulance.png",
"files/sprite/audi.png",
"files/sprite/truck.png"};

static SDL_Texture* sprites[NSprite];
static const char* sprites_files[] = { 
"files/sprite/frog.png"
};

enum { SZ = 40 };
enum { FPS = 15 };


static int load_sprites(void) {
    int i;
    SDL_Surface *png;
    for(i = 0; i < NSprite; ++i) {
        png = IMG_Load(sprites_files[i]);
        if (png == NULL){
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            printf("Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        sprites[i] = SDL_CreateTextureFromSurface(ren, png);
        SDL_FreeSurface(png);
    }
    return 0;
}

static int load_tiles(void) {
    int i;
    SDL_Surface *png;
    for(i = 0; i < NCell; ++i) {
        png = IMG_Load(tiles_files[i]);
        if (png == NULL){
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            printf("Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        tiles[i] = SDL_CreateTextureFromSurface(ren, png);
        SDL_FreeSurface(png);
    }
    return 0;
}

static int init(const Game* game) {
    int i;

    GAME = game;
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL_Init Error: %s\n",  SDL_GetError());
        return 1;
    }
    
    win = SDL_CreateWindow("Game", 0, 0, GAME->w * SZ, GAME->h * SZ, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL){
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    if(load_tiles())
        return 1;
    
    if(load_sprites())
        return 1;
    
    return 0;
}

static void start(void(*callback)(void*)) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(callback, &sdl_driver, FPS, 0);
#else
    for(;;) {
        callback(&sdl_driver);
        clock_t debut = clock();
        usleep(100000);
        clock_t ecoule = clock() - debut;
    }
#endif
}

static int get_move(void) {
    SDL_PumpEvents();
    const Uint8 * state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_P]) {
        SDL_Quit();
        exit(0);
    }
    if (state[SDL_SCANCODE_W]){
        return Up;    
    }
    if (state[SDL_SCANCODE_S]) {
        return Down;
    }
    if (state[SDL_SCANCODE_A]){
        return Left;
    }
    if (state[SDL_SCANCODE_D]){
        return Right;
    }
    return Nothing;
}

static void draw_bg(void) {
    SDL_RenderClear(ren);
    int y, x;
    SDL_Rect dst = {.x = 0, .y = 0, .w = SZ, .h = SZ };
    for(y = 0; y < GAME->h; ++y) {
        for(x = 0; x < GAME->w; ++x) {
            dst.x = x * SZ;
            dst.y = y * SZ;
            int typecell = GAME->background[y * GAME->w + x];
            SDL_RenderCopy(ren, tiles[typecell], NULL, &dst);
        }
    }
}

static void draw_entity(int ent_id) {
    static int sp = 0;
    if (ent_id == 6 || ent_id == 7){
        SDL_Rect src = {.x = 0, .y = 0, .w = SZ, .h = SZ };
        SDL_Rect dst = {.x = SZ* GAME->entity[ent_id].x, .y = SZ * GAME->entity[ent_id].y, .w = SZ, .h = SZ };
        src.x = sp * 40;
        SDL_RenderCopy(ren, sprites[ent_id], &src, &dst);
        sp = (sp + 1) % 3;
    }
    else {
        SDL_Rect src = {.x = 0, .y = 0, .w = SZ, .h = SZ };
        SDL_Rect dst = {.x = SZ* GAME->entity[ent_id].x, .y = SZ * GAME->entity[ent_id].y, .w = SZ, .h = SZ };
        SDL_RenderCopy(ren, sprites[ent_id], &src, &dst);
    }
}

static void update(void) {
    SDL_RenderPresent(ren);
}

