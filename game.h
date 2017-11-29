typedef struct Game Game;
typedef struct Driver Driver;
typedef struct Entity Entity;

struct Entity {
    int x;
    int y;
    int id;
    int dir;
};

struct Game {
    int h;
    int w;
    unsigned char* background;
    Entity* entity;
};

enum {
    Grass,      //A
    Water,      //B
    Brique1,    //C
    Brique2,    //D
    Street,     //E
    Taxi,       //F
    Mini,       //G
    Police,     //H
    Ambulance,  //I
    Audi,       //J
    Truck,      //K
    NCell
};

enum {
    Bender,
    NSprite
};

enum {
    Nothing, Left, Right, Up, Down
};

void init_game(const Driver*);

