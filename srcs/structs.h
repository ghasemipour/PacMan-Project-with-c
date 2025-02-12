typedef struct menuOption
{
    int name;
    SDL_Surface *offImg;
    SDL_Surface *onImg;
    bool isOn;
    struct menuOption *nxt;
    struct menuOption *pre;
}menuOption;

typedef struct coordinate
{
    float x, y;
}coordinate;

typedef struct ghost
{
    SDL_Surface *img;
    int type;
    int direction;
    coordinate pos;
}ghost;

typedef struct record_t
{
    char nickname[MaxLen];
    int score;
}record_t;