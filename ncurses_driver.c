#include "game.h"
#include "driver.h"
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>

static int init(const Game* game);
static void start(void(*callback)(void*));
static int get_move(void);
static void draw_bg(void);
static void draw_entity(int ent_id);
static void update(void);

Driver ncurses_driver = {
    .game = NULL,
    .init = init,
    .start = start,
    .get_move = get_move,
    .draw_bg = draw_bg,
    .draw_entity = draw_entity,
    .update = update
};

#define GAME (ncurses_driver.game)

static char tiles[] = {'|', ':',':', '~','-','A','B','C','D','E','F','C','D','E','F'};
static const char* sprites[NSprite] = {"ooo@<<<@>>>@^^^@vvv@"};

enum { FPS = 15 };

static int init(const Game* game) {
    GAME = game;
    initscr();
    curs_set(0);
    noecho();
    halfdelay(1);
    return 0;
}

static void start(void(*callback)(void*)) {
    for(;;) {
        callback(&ncurses_driver);
        clock_t debut = clock();
        usleep(100000);
        clock_t ecoule = clock() - debut;
    }
}

static int get_move(void) {
    int car = getch();
    switch(car) {
        case 'z':
            return Up;
        case 'q':
            return Left;
        case 's':
            return Down;
        case 'd':
            return Right;
        case 'p':
              refresh();
              endwin();
              exit(0);

        default:
            break;
    }
    return Nothing;
}

static void draw_bg(void) {
    int y, x;
    for(y = 0; y < GAME->h; ++y) {
        for(x = 0; x < GAME->w; ++x) {
            int typecell = GAME->background[y * GAME->w + x];
            mvprintw(y, x, "%c", tiles[typecell]);
        }
    }
}

static void draw_entity(int ent_id) {
    static int anim = 0;
    mvprintw(GAME->entity[ent_id].y, GAME->entity[ent_id].x, "%c", sprites[ent_id][4 * GAME->entity[ent_id].dir + anim]);
    anim = (anim + 1) % 1;
}

static void update(void) {
    refresh();
}
