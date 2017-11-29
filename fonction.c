/*
static void collision(void) {
    SDL_Event event;
    int pause = 0;
    for(int i = 1; i < NSprite ; i++){
        if (GAME->entity[0].x == GAME->entity[i].x && GAME->entity[0].y == GAME->entity[i].y){
            while(pause != 1 && SDL_WaitEvent(&event)){
                switch (event.type) {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                            case 'm':
                                GAME->entity[0].x = 14;
                                GAME->entity[0].y = 14;       
                                pause = 1;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;                            
                }
            }
        }
    }

}
*/



