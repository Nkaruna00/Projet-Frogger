#include "game.h"
#include "driver.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define H 16
#define W 27

Game g;

static const char* level1 = {
    "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AACCCCCCCCCCCCCCCCCCCCCCCAA"
    "AACCCECCCECCCECCCECCCECCCAA"
    "AADDDDDDDDDDDDDDDDDDDDDDDAA"
    "AADDDDDDDDDDDDDDDDDDDDDDDAA"
    "AADDDDDDDDDDDDDDDDDDDDDDDAA"
    "AADDDDDDDDDDDDDDDDDDDDDDDAA"
    "AADDDDDDDDDDDDDDDDDDDDDDDAA"
    "AABBBBBBBBBBBBBBBBBBBBBBBAA"
    "AAEEEEEEEEEEEEEEEEEEEEEEEAA"
    "AAEEEEEEEEEEEEEEEEEEEEIEEAA"
    "AAEEEEEEEEEEEEEEHEEEEEEEHAA"
    "AAEEEEEEEEEGEEEEEEEEFEEEEAA"
    "AAEEEEEEEEEEEEEEEEEEEEEEEAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
};

static unsigned char bg[H * W];

static Entity entity[1];

static int dirx[] = {0, -1, 1, 0, 0 };
static int diry[] = {0, 0, 0, -1, 1 };

static void process_move(int move) {
    entity[0].dir = move;
    int nextx = entity[0].x + dirx[move];
    int nexty = entity[0].y + diry[move];
    entity[0].dir = move;
    if(nextx < 0 || nextx >= W || nexty < 0 || nexty >= H || bg[nexty * W + nextx] == Grass|| bg[nexty * W + nextx] == Brique1)
        return;
    entity[0].x = nextx;
    entity[0].y = nexty;
}
/*
static void move_auto(){
    int tmp;
    for(int i = 0; i < H*W; i++){
		if(g.background[i] == Taxi ){
	        tmp = g.background[i];
	        g.background[i] = g.background[i-1];
	        g.background[i-1] = tmp ;
	        if( g.background[i] ){
		        return;
		    }
		}
	}
}*/

static void move_auto(){
    int tmp;
    int tmp2;
    for(int i = 0; i < H*W; i++){
		if(g.background[i] == Taxi|g.background[i] == Mini|g.background[i] == Police){
	        if( g.background[i-1] == Grass ){
		        tmp = g.background[i];
	            g.background[i] = g.background[i+(W-5)];
		        g.background[i+(W-5)] = tmp ;
		    }

		    else{
		        tmp = g.background[i];
	            g.background[i] = g.background[i-1];
		        g.background[i-1] = tmp ;
		    }
		}

    if(g.background[i] == Ambulance){
	        if( g.background[i+1] == Grass ){
		        tmp2 = g.background[i];
	            g.background[i] = g.background[i+(W-27)];
		        g.background[i+(W-27)] = tmp2 ;
		    }

		    else{
		        tmp2 = g.background[i];
	            g.background[i] = g.background[i+1];
		        g.background[i+1] = tmp2 ;
		    }
		}


	}
}

static void collision(int move) {
    entity[0].dir = move;
    int nextx = entity[0].x + dirx[move];
    int nexty = entity[0].y + diry[move];

    if(bg[nexty * W + nextx] == Taxi || bg[nexty * W + nextx] == Mini){
            //sleep(1);
            g.entity[0].x = 14;
            g.entity[0].y = 14;
    }
}

static void callback(void* d) {
    const Driver* dr = (Driver*)d;
    int move = dr->get_move();
    dr->draw_bg();
    for(int i=0; i<NSprite;++i){
        dr->draw_entity(i);
    }
    process_move(move);
    move_auto();
    collision(move);
    dr->update();
}

void init_game(const Driver* dr) {
    int i,x,y;
    g.h = H;
    g.w = W;
    g.background = bg;
    g.entity = entity;

    for(i = 0; i < H * W; ++i){
        g.background[i] = level1[i] - 'A';
    }


    g.entity[0].x = 14;
    g.entity[0].y = 14;
    g.entity[0].id = 0;

    /*
    for(i=1,x=8,y=13 ; i<=4 ;i++, x+=6){
        g.entity[i].x = x;
        g.entity[i].y = y;
        g.entity[i].id = i;
    }

    for(i=5,x=14,y=12 ; i<=7 ;i++, x-=6){
        g.entity[i].x = x;
        g.entity[i].y = y;
        g.entity[i].id = i;
    }

    g.entity[8].x = 22;
    g.entity[8].y = 11;
    g.entity[8].id = 8;

    for(i=9,x=13,y=10 ; i<=11 ;i++, x-=11){
        g.entity[i].x = x;
        g.entity[i].y = y;
        g.entity[i].id = i;
    }*/

    dr->init(&g);
    dr->start(callback);
}

// 4 3 1 2
