coordinate GetPos()
{
    coordinate res;
    while(true)
    {
        res.y = (int)(rand() % MapHeight);
        res.x = (int)(rand() % MapWidth);
        if (map[mapNum][(int) res.y][(int) res.x] == FREE)
                break;
    }
    return res;
}
coordinate GetPosGhost()
{
    coordinate res;
    while(true)
    {
        res.y = (int)(rand() % MapHeight);
        res.x = (int)(rand() % MapWidth);
        if (map[mapNum][(int) res.y][(int) res.x] == FREE && shortestPath[(int)res.y][(int)res.x][(int)pacmanPos.y][(int)pacmanPos.x] > 5)
            break;
    }
    return res;
}

void InitMap()
{
    for(int i = 0; i < MapHeight; i++)
    {
        for(int j = 0; j < MapWidth; j++)
        {
            if(map[mapNum][i][j] >= STAR)
                map[mapNum][i][j] = FREE;
        }
    }
}

void InitStars()
{
    for(int i = 0; i < numOfStars; i++)
    {
        coordinate starPos = GetPos();
        map[mapNum][(int)starPos.y][(int)starPos.x] = STAR;
    }
}

void InitGhosts()
{
    char paths[][MaxLen] = {
            "../images/game/ghost1.png",
            "../images/game/ghost2.png",
            "../images/game/ghost3.png",
            "../images/game/ghost4.png",
            "../images/game/ghost5.png",
            "../images/game/ghost6.png",
            "../images/game/ghost7.png"
    };

    //Chasing
    for(int i = 0; i < numOfGhosts / 2; i++)
    {
        ghosts[i].img = IMG_Load(paths[i]);
        ghosts[i].type = (i == 0) ? CHASING : RANDOM;
        ghosts[i].pos.y = initGhostRow[mapNum][0];
        ghosts[i].pos.x = (float)12 + (float)i;
        ghosts[i].direction = UP;
    }

    //Random
    for(int i = numOfGhosts / 2; i < numOfGhosts; i++)
    {
        ghosts[i].img = IMG_Load(paths[i]);
        ghosts[i].type = RANDOM;
        ghosts[i].pos.y = initGhostRow[mapNum][1];
        ghosts[i].pos.x = (float)12 + (float)(i - numOfGhosts / 2);
        ghosts[i].direction = UP;
    }
}