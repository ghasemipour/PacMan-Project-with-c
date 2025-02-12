int shortestPath[MapHeight][MapWidth][MapHeight][MapWidth]; // shortest path from (i, j) to (l, k)

void InitShortestPath()
{
    for(int i = 0; i < MapHeight; i++)
    {
        for(int j = 0; j < MapWidth; j++)
        {
            for(int l = 0; l < MapHeight; l++)
            {
                for(int k = 0; k < MapWidth; k++)
                {
                    shortestPath[i][j][l][k] = Inf;
                }
            }

            if(map[mapNum][i][j] == WALL || map[mapNum][i][j] == GATE)
                continue;

            shortestPath[i][j][i][j] = 0;

            if(map[mapNum][(i+1)%MapHeight][j] != WALL && map[mapNum][(i+1)%MapHeight][j] != GATE)
                shortestPath[i][j][(i+1)%MapHeight][j] = 1;

            if(map[mapNum][(i-1+MapHeight)%MapHeight][j] != WALL && map[mapNum][(i-1+MapHeight)%MapHeight][j] != GATE)
                shortestPath[i][j][(i-1+MapHeight)%MapHeight][j] = 1;

            if(map[mapNum][i][(j+1)%MapWidth] != WALL && map[mapNum][i][(j+1)%MapWidth] != GATE)
                shortestPath[i][j][i][(j+1)%MapWidth] = 1;

            if(map[mapNum][i][(j-1+MapWidth)%MapWidth] != WALL && map[mapNum][i][(j-1+MapWidth)%MapWidth] != GATE)
                shortestPath[i][j][i][(j-1+MapWidth)%MapWidth] = 1;
        }
    }

    return;
}

void FindShortestPath()
{
    InitShortestPath();

    for(int t = 0; t < MapWidth + MapHeight; t++)
    {
        for(int i = 0; i < MapHeight; i++)
            for(int j = 0; j < MapWidth; j++)
            {
                if(map[mapNum][i][j] == WALL || map[mapNum][i][j] == GATE)
                    continue;
                for(int l = 0; l < MapHeight; l++)
                    for(int k = 0; k < MapWidth; k++)
                    {
                        if(map[mapNum][l][k] == WALL || map[mapNum][l][k] == GATE)
                            continue;

                        for(int dy = -1; dy <= 1; dy++)
                        {
                            for(int dx = -1; dx <= 1; dx++)
                            {
                                if(dy != 0 && dx != 0)
                                    continue;

                                if(dy == 0 && dx == 0)
                                    continue;

                                int y = (l + dy + MapHeight) % MapHeight;
                                int x = (k + dx + MapWidth) % MapWidth;

                                if(map[mapNum][y][x] == WALL || map[mapNum][y][x] == GATE || shortestPath[i][j][y][x] >= Inf)
                                    continue;

                                shortestPath[i][j][l][k] = fmin(shortestPath[i][j][l][k], shortestPath[i][j][y][x] + 1);
                                shortestPath[i][j][l][k] = fmin(shortestPath[i][j][l][k], Inf);
                            }
                        }
                    }
            }

    }


    return;
}