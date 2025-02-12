bool CmpPos(coordinate pos1, coordinate pos2)
{
    return (((int)pos1.x == (int)pos2.x) && ((int)pos1.y == (int)pos2.y));
}

coordinate MovePacman(int direction)
{
    coordinate newPos = pacmanPos;
    switch (direction)
    {
        case UP:
            newPos.y -= deltaTime * pacmanSpeed;
            break;
        case DOWN:
            newPos.y += deltaTime * pacmanSpeed;
            break;
        case LEFT:
            newPos.x -= deltaTime * pacmanSpeed;
            break;
        case RIGHT:
            newPos.x += deltaTime * pacmanSpeed;
            break;
        default:
            break;
    }

    if(newPos.x < 0 && map[mapNum][(int)newPos.y][0] == FREE)
        newPos.x += MapWidth;
    if(newPos.x >= MapWidth && map[mapNum][(int)newPos.y][0] == FREE)
        newPos.x -= MapWidth;

    if(map[mapNum][(int)newPos.y][(int)newPos.x] != WALL && map[mapNum][(int)newPos.y][(int)newPos.x] != GATE)
        return newPos;

    return pacmanPos;
}

coordinate MoveGhost(int num)
{
    coordinate newPos = ghosts[num].pos;
    int x = ghosts[num].pos.x, y = ghosts[num].pos.y;
    int t = 10;
    if(map[mapNum][y][x] == GATE)
    {
        if(newPos.x < 13)
            newPos.x += deltaTime * ghostSpeed;
        else if(newPos.x > 14)
            newPos.x -= deltaTime * ghostSpeed;
        else
            newPos.y -= deltaTime * ghostSpeed;

        return newPos;
    }
    int dx = 0 , dy = 0;
    float speed = ghostSpeed;
    int dir = ghosts[num].direction;
    if(cherry > 0)
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i != 0 && j != 0)
                    continue;
                if(i == 0 && j == 0)
                    continue;

                if(shortestPath[(y+j+MapHeight)%MapHeight][(x+i+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x] >= Inf)
                    continue;

                if(dx == 0 && dy == 0)
                {
                    dx = i;
                    dy = j;
                    continue;
                }
                if(shortestPath[(y+j+MapHeight)%MapHeight][(x+i+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x] >= shortestPath[(y+dy+MapHeight)%MapHeight][(x+dx+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x])
                {
                    dx = i;
                    dy = j;
                }

            }
        }
    }
    else
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i != 0 && j != 0)
                    continue;

                if(i == 0 && j == 0)
                    continue;

                if(shortestPath[(y+j+MapHeight)%MapHeight][(x+i+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x] >= Inf)
                    continue;

                if(dx == 0 && dy == 0)
                {
                    dx = i;
                    dy = j;
                    continue;
                }

                if(shortestPath[(y+j+MapHeight)%MapHeight][(x+i+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x] < shortestPath[(y+dy+MapHeight)%MapHeight][(x+dx+MapWidth)%MapWidth][(int)pacmanPos.y][(int)pacmanPos.x])
                {
                    dx = i;
                    dy = j;
                }

            }
        }
    }



    switch (ghosts[num].type)
    {
        case CHASING:
            newPos.x += (float)dx * deltaTime * ghostSpeed;
            newPos.y += (float)dy * deltaTime * ghostSpeed;
            if(newPos.x < ghosts[num].pos.x)
                dir = LEFT;
            else if(newPos.x > ghosts[num].pos.x)
                dir = RIGHT;
            else if(newPos.y < ghosts[num].pos.y)
                dir = UP;
            else
                dir = DOWN;
            if(newPos.x < 0)
                newPos.x += MapWidth;
            if(newPos.x >= MapWidth)
                newPos.x -= MapWidth;

            if(newPos.y < 0)
                newPos.y += MapHeight;
            if(newPos.y >= MapHeight)
                newPos.y -= MapHeight;

            ghosts[num].direction = dir;
            break;
        case RANDOM:
            while (true)
            {
                int type = (rand() + num) % 500 + 1;
                if(type > 4)
                {
                    type = dir;
                }
                if(type * dir == 2 || type * dir == 12)
                    type = dir;
                dir = type;
                switch (type)
                {
                    case UP:
                        newPos.y -= deltaTime * speed;
                        break;
                    case DOWN:
                        newPos.y += deltaTime * speed;
                        break;
                    case LEFT:
                        newPos.x -= deltaTime * speed;
                        if(newPos.x < 0 && map[mapNum][y][0] == FREE)
                            newPos.x += MapWidth;
                        break;
                    case RIGHT:
                        newPos.x += deltaTime * speed;
                        if(newPos.x >= MapWidth && map[mapNum][y][0] == FREE)
                            newPos.x -= MapWidth;
                        break;
                }
                if(newPos.y >= 0 && newPos.x < MapHeight && newPos.x >= 0 && newPos.x < MapWidth && map[mapNum][(int)newPos.y][(int)newPos.x] != WALL && map[mapNum][(int)newPos.y][(int)newPos.x] != GATE)
                    break;

                dir = UP + rand() % 4;
                newPos = ghosts[num].pos;
            }
            ghosts[num].direction = dir;
            break;
        default:
            break;
    }

    return newPos;
}

void AddFruit()
{
    int type = (rand() % 4) + CHERRY;
    coordinate pos;
    pos.y = (int)(rand() % MapHeight);
    pos.x = (int)(rand() % MapWidth);
    if (map[mapNum][(int) pos.y][(int) pos.x] == FREE)
        map[mapNum][(int)pos.y][(int)pos.x] = type;
}