void InitGameLoop(int difficulty)
{
    FILE *fptr = fopen("../texts/MapFrames.txt", "w");
    mapNum = (mapNum + 1) % NumberOfMaps;
    fprintf(fptr, "%d\n", mapNum);
    fflush(fptr);
    fclose(fptr);

    fptr = fopen("../texts/PacmanFrames.txt", "w");
    fflush(fptr);
    fclose(fptr);

    fptr = fopen("../texts/Records.txt", "r");
    char tmp[MaxLen];
    fgets(tmp, MaxLen, fptr);
    fscanf(fptr, "%d", &bestScore);
    FindShortestPath();
    life = 3;
    score = 0;
    InitMap();
    switch (difficulty)
    {
        case EASY:
            numOfGhosts = 5;
            numOfStars = 10;
            ghostSpeed = 3;
            pacmanSpeed = 5;
            break;
        case NORMAL:
            numOfGhosts = 6;
            numOfStars = 15;
            ghostSpeed = 3;
            pacmanSpeed = 8;
            break;
        case HARD:
            numOfGhosts = 7;
            numOfStars = 20;
            ghostSpeed = 4;
            pacmanSpeed = 10;
            break;
        default:
            break;
    }

    fptr = fopen("../texts/GhostsFrames.txt", "w");
    fprintf(fptr, "%d\n", numOfGhosts);
    fflush(fptr);
    fclose(fptr);

    pacmanPos = initPacmanPos[mapNum];
    InitStars();
    InitGhosts();
}

void UpdateGameLoop()
{
    death = false;
    static int pacmanDirection = NONE;
    if(lastPressedKey != NONE && lastPressedKey != ENTER)
        pacmanDirection = lastPressedKey;
    else if(pacmanDirection == NONE)
        return;

    static int remainingStars = -1;
    if(score == 0)
        remainingStars = numOfStars;

    if(life <= 0)
    {
        pacmanDirection = NONE;
        AddRecord();
        gameStatus = GAME_OVER;
        return;
    }

    if(remainingStars <= 0)
    {
        InitStars();
        remainingStars = numOfStars;
    }

    cherry = (float)fmax(0, cherry - deltaTime);
    if(speedBoostTime && speedBoostTime - deltaTime <= 0)
        pacmanSpeed -= BoostSpeed;
    speedBoostTime = (float)fmax(0, speedBoostTime - deltaTime);

    // checking fruits vs pacman
    switch (map[mapNum][(int)pacmanPos.y][(int)pacmanPos.x])
    {
        case STAR:
            score += 100;
            remainingStars--;
            break;
        case CHERRY:
            cherry = CherryTime;
            break;
        case PEPPER:
            if(!speedBoostTime)
                pacmanSpeed += BoostSpeed;
            speedBoostTime = BoostTime;
            break;
        case APPLE:
            life = (int)fmin(3, life + 1);
            break;
        case MUSHROOM:
            death = true;
            life--;
            break;
        default:
            break;
    }
    if(map[mapNum][(int)pacmanPos.y][(int)pacmanPos.x] > GATE)
        map[mapNum][(int)pacmanPos.y][(int)pacmanPos.x] = FREE;

    //checking ghosts vs pacman
    for(int i = 0; i < numOfGhosts; i++)
    {
        if(CmpPos(pacmanPos, ghosts[i].pos))
        {
            if(cherry > 0)
            {
                score += 150;
            }
            else
            {
                death = true;
                life--;
            }
            ghosts[i].pos.x = (float)(12 + ((i < numOfGhosts / 2) ? i : i - numOfGhosts / 2));
            ghosts[i].pos.y = (i < numOfGhosts / 2) ? initGhostRow[mapNum][0] : initGhostRow[mapNum][1];
            break;
        }
    }

    if(death)
    {
        pacmanPos = initPacmanPos[mapNum];
        pacmanDirection = NONE;
    }

    if((int)(rand() % 25) == 0)
        AddFruit();

    // moving objects:
    pacmanPos = MovePacman(pacmanDirection);
    for(int i = 0; i < numOfGhosts; i++)
        ghosts[i].pos = MoveGhost(i);

    bestScore = fmax(bestScore, score);
}

void RenderGameLoop()
{
    SDL_Surface *loadedSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    float h = (float)screenHeight / (MapHeight + 4);
    float w = (float)screenWidth / (MapWidth + 4);

    // path of all we need
    char mapPaths[NumberOfMaps][MaxLen] = {
      "../images/game/map1.png",
      "../images/game/map2.png",
      "../images/game/map3.png"
    };

    char heartPath[MaxLen] = "../images/game/heart.png";

    char ghostD[MaxLen] = "../images/game/ghostD.png";

    char pacmanPath[2][4][MaxLen] = {
            "../images/game/pacman1up.png",
            "../images/game/pacman1down.png",
            "../images/game/pacman1left.png",
            "../images/game/pacman1right.png",
            "../images/game/pacman2up.png",
            "../images/game/pacman2down.png",
            "../images/game/pacman2left.png",
            "../images/game/pacman2right.png",
    };

    char objectPath[5][MaxLen] ={
            "../images/game/star.png",
            "../images/game/cherry.png",
            "../images/game/pepper.png",
            "../images/game/apple.png",
            "../images/game/poisonMushroom.png"

    };

    // render map :
    loadedSurface = IMG_Load(mapPaths[mapNum]);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", mapPaths[mapNum], IMG_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = w * MapWidth;
    dest.h = h * MapHeight;
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = (screenHeight - dest.h) / 2;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    // render life :
        // heart image:
    loadedSurface = IMG_Load(heartPath);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", heartPath, IMG_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = w;
    dest.h = h;
    dest.y = 0;
    dest.x = screenWidth - w;
    for(int i = 0; i < life; i++)
    {
        dest.x -= w;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    }
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;


    //render score:
    SDL_Color color2 = {225, 225, 225, 225};
    char scores[MaxLen], tmp[MaxLen];
    strcpy(scores, "SCORE : ");
    sprintf(tmp, "%d", score);
    strcat(scores, tmp);
    loadedSurface = TTF_RenderText_Solid(font2, scores, color2);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "SCORE", TTF_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = 2*w;
    dest.y = 0;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    //render best score:
    char best_score[MaxLen] = "BEST SCORE : ";
    sprintf(tmp, "%d", bestScore);
    strcat(best_score, tmp);
    loadedSurface = TTF_RenderText_Solid(font2, best_score, color2);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", " BEST SCORE", TTF_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = 0;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    //render objects:
    for(int x = 0; x < MapWidth; x++)
        for (int y = 0; y < MapHeight; y++)
        {
            if(map[mapNum][y][x] < STAR)
                continue;

            loadedSurface = IMG_Load(objectPath[map[mapNum][y][x]-STAR]);
            if(loadedSurface == NULL)
            {
                printf( "Unable to load image %s! SDL_image Error: %s\n", objectPath[map[mapNum][y][x]-STAR], IMG_GetError() );
                return;
            }
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
            dest.w = w;
            dest.h = h;
            dest.x = x * w + 2*w;
            dest.y = y * h + 2*h;
            SDL_RenderCopy(gRenderer, tex, NULL, &dest);
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;
            SDL_DestroyTexture(tex);
            tex = NULL;
        }

    //render pacman:
    static int tmp_time= 0;
    tmp_time++;
    tmp_time %= 10;
    int type = (tmp_time < 5) ? 0 : 1;
    static int pacmanDirection = RIGHT;
    if(lastPressedKey != NONE && lastPressedKey != ENTER)
        pacmanDirection = lastPressedKey;
    int dir = pacmanDirection - UP;
    loadedSurface = IMG_Load(pacmanPath[(int)type][dir]);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", pacmanPath[(int)type][dir], IMG_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = w;
    dest.h = h;
    dest.x = (int) (pacmanPos.x) * w + 2 * w;
    dest.y = (int) (pacmanPos.y) * h + 2 * h;
    /*if(map[mapNum][(int)(pacmanPos.y)][(int)(pacmanPos.x + 1)] == WALL || map[mapNum][(int)(pacmanPos.y)][(int)(pacmanPos.x + 1)] == GATE)
    {
        dest.x = (int) (pacmanPos.x) * w + 2 * w;
        if(dir + UP != LEFT && dir + UP != RIGHT)
            pacmanPos.x = (int) pacmanPos.x;
    }
    else
        dest.x = (pacmanPos.x) * w + 2*w;
    if(map[mapNum][(int)(pacmanPos.y + 1)][(int)(pacmanPos.x)] == WALL || map[mapNum][(int)(pacmanPos.y + 1)][(int)(pacmanPos.x)] == GATE)
    {
        dest.y = (int) (pacmanPos.y) * h + 2 * h;
        if(dir + UP != UP && dir + UP != DOWN)
            pacmanPos.y = (int) pacmanPos.y;
    }
    else
        dest.y = (pacmanPos.y) * h + 2*h;*/
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    //render ghosts:
    for(int i = 0; i < numOfGhosts; i++)
    {
        if(!cherry)
            tex = SDL_CreateTextureFromSurface(gRenderer, ghosts[i].img);
        else
        {
            loadedSurface = IMG_Load(ghostD);
            if(loadedSurface == NULL)
            {
                printf( "Unable to load image %s! SDL_image Error: %s\n", ghostD, IMG_GetError() );
                return;
            }
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        }
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = w;
        dest.h = h;
        dest.x = (int)(ghosts[i].pos.x) * w + 2*w;
        dest.y = (int)(ghosts[i].pos.y) * h + 2*h;
        /*if(map[mapNum][(int)(ghosts[i].pos.y)][(int)(ghosts[i].pos.x + 1)] == WALL)
        {
            dest.x = (int) (ghosts[i].pos.x) * w + 2 * w;
            if(ghosts[i].direction + UP != LEFT && ghosts[i].direction + UP != RIGHT)
                ghosts[i].pos.x = (int) ghosts[i].pos.x;
        }
        else
            dest.x = (ghosts[i].pos.x) * w + 2*w;
        if(map[mapNum][(int)(ghosts[i].pos.y + 1)][(int)(ghosts[i].pos.x)] == WALL)
        {
            dest.y = (int) (ghosts[i].pos.y) * h + 2 * h;
            if(ghosts[i].direction + UP != UP && ghosts[i].direction + UP != DOWN)
                ghosts[i].pos.y = (int) ghosts[i].pos.y;
        }
        else
            dest.y = (ghosts[i].pos.y) * h + 2*h;*/
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
        SDL_DestroyTexture(tex);
        tex = NULL;
    }

    //render boosts:
    if(cherry)
    {
        loadedSurface = IMG_Load(objectPath[CHERRY-STAR]);
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", objectPath[CHERRY-STAR], IMG_GetError() );
            return;
        }
        tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = w;
        dest.h = h;
        dest.x = 2*w;
        dest.y = screenHeight - 1.5*h;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
        SDL_DestroyTexture(tex);
        tex = NULL;
    }
    if(speedBoostTime)
    {
        loadedSurface = IMG_Load(objectPath[PEPPER-STAR]);
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", objectPath[PEPPER-STAR], IMG_GetError() );
            return;
        }
        tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = w;
        dest.h = h;
        dest.x = 4*w;
        dest.y = screenHeight - 1.5*h;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
        SDL_DestroyTexture(tex);
        tex = NULL;
    }

    //Play Musics :
    if(cherry)
    {
        DestroyGameBackgroundMusic();
        PlayEatGhostMusic();
    }
    else
    {
        DestroyEatGhostMusic();
        PlayGameBackgroundMusic();
    }

    //Play Sounds:
    if(map[mapNum][(int)pacmanPos.y][(int)pacmanPos.x] > STAR)
    {
        PlayEatFruitSound();
    }
    else if(map[mapNum][(int)pacmanPos.y][(int)pacmanPos.x] == STAR)
    {
        PlayCollectStarSound();
    }
    if(death)
    {
        PlayDeathSound();
    }
    SDL_RenderPresent(gRenderer);

    AddFrame(pacmanDirection);
}

void CLearGameLoop()
{
    for(int i = 0; i < numOfGhosts; i++)
    {
        SDL_FreeSurface(ghosts[i].img);
    }
    DestroyGameMusicsAndSounds();
    return;
}