enum GameStatus
{
    START_SCREEN,
    MENU,
    GAME_LOOP,
    RECORDS,
    EXIT,
    ENTER_NICKNAME,
    CHOSE_DIFFICULTY,
    GAME_OVER,
    REPLAY
};

enum LastPressedKey
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER
};

enum GameDifficulty
{
    EASY,
    NORMAL,
    HARD
};

enum GameObjects
{
    FREE,
    WALL,
    GATE,
    STAR,
    CHERRY,
    PEPPER,
    APPLE,
    MUSHROOM
};

enum GhostType
{
    CHASING,
    RANDOM
};
