void AddFrame(int pacmanDir)
{
    FILE *fptr = NULL;
    FILE *ftmp = NULL;

    //Add Map Frame
    fptr = fopen("../texts/MapFrames.txt", "a");
    if(!fptr) {
        printf("Failed to open map frames file!\n");
        return;
    }
    fseek(fptr, 0, SEEK_END);
    for(int i = MapHeight - 1; i >= 0; i--)
    {
        for(int j = MapWidth - 1; j >= 0; j--)
        {
            fprintf(fptr, "%d ", map[mapNum][i][j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);

    //Add Pacman Frame
    fptr = fopen("../texts/PacmanFrames.txt", "a");
    if(!fptr)
    {
        printf("Failed to open map pacman file!\n");
        return;
    }
    fseek(fptr, 0, SEEK_END);
    fprintf(fptr, "%f %f %d\n", pacmanPos.x, pacmanPos.y, pacmanDir);
    fclose(fptr);

    //Add Ghosts Frame
    fptr = fopen("../texts/GhostsFrames.txt", "a");
    if(!fptr)
    {
        printf("Failed to open map ghosts file!\n");
        return;
    }
    fseek(fptr, 0, SEEK_END);
    for(int i = numOfGhosts - 1; i >= 0; i--)
    {
        fprintf(fptr, "%f %f\n", ghosts[i].pos.x, ghosts[i].pos.y);
    }
    fclose(fptr);

}

void ShowReplay()
{
    char mapPaths[NumberOfMaps][MaxLen] = {
            "../images/game/map1.png",
            "../images/game/map2.png",
            "../images/game/map3.png"
    };

    char ghostsPath[][MaxLen] = {
            "../images/game/ghost1.png",
            "../images/game/ghost2.png",
            "../images/game/ghost3.png",
            "../images/game/ghost4.png",
            "../images/game/ghost5.png",
            "../images/game/ghost6.png",
            "../images/game/ghost7.png"
    };

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

    SDL_Surface *loadedSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    float h = (float)screenHeight / (MapHeight + 4);
    float w = (float)screenWidth / (MapWidth + 4);

    FILE *fmap = fopen("../texts/MapFrames.txt", "r");
    FILE *fpacman = fopen("../texts/PacmanFrames.txt", "r");
    FILE *fghosts = fopen("../texts/GhostsFrames.txt", "r");

    int map_num, numGhosts;
    fscanf(fmap, "%d\n", &map_num);
    fscanf(fghosts, "%d\n", &numGhosts);
    int newMap[MapHeight][MapWidth], type = 0;
    float pacman_x, pacman_y;
    int pacman_dir;
    float ghost_x[numGhosts], ghost_y[numGhosts];

    PlayReplayMusic();
    while(!feof(fmap))
    {

        for(int i = MapHeight - 1; i >= 0; i--)
        {
            for(int j = MapWidth - 1; j >= 0; j--)
            {
                fscanf(fmap, " %d", &newMap[i][j]);
            }
            fgetc(fmap);
        }


        fscanf(fpacman, "%f %f %d\n", &pacman_x, &pacman_y, &pacman_dir);

        for(int i = numGhosts - 1; i >= 0; i--)
            fscanf(fghosts, "%f %f\n", &ghost_x[i], &ghost_y[i]);


        //rendering:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
        SDL_RenderClear(gRenderer);

        //render replay icon
        loadedSurface = IMG_Load("../images/replay.png");
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", "../images/replay.png", IMG_GetError() );
            return;
        }
        tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = 2*w;
        dest.h = 2*h;
        dest.x = 2*w;
        dest.y = 0;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
        SDL_DestroyTexture(tex);
        tex = NULL;

        // render map :
        loadedSurface = IMG_Load(mapPaths[map_num]);
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", mapPaths[map_num], IMG_GetError() );
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

        //render objects:
        for(int x = 0; x < MapWidth; x++)
            for (int y = 0; y < MapHeight; y++)
            {
                if(newMap[y][x] < STAR)
                    continue;

                loadedSurface = IMG_Load(objectPath[newMap[y][x]-STAR]);
                if(loadedSurface == NULL)
                {
                    printf( "Unable to load image %s! SDL_image Error: %s\n", objectPath[newMap[y][x]-STAR], IMG_GetError() );
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
        type = (type + 1) % 4;
        loadedSurface = IMG_Load(pacmanPath[(type > 1) ? 1 : 0][pacman_dir - UP]);
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", pacmanPath[(type > 1) ? 1 : 0][pacman_dir - UP], IMG_GetError() );
            return;
        }
        tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = w;
        dest.h = h;
        dest.x = (int)(pacman_x) * w + 2*w;
        dest.y = (int)(pacman_y) * h + 2*h;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
        SDL_DestroyTexture(tex);
        tex = NULL;

        //render ghosts:
        for(int i = 0; i < numGhosts; i++)
        {
            loadedSurface = IMG_Load(ghostsPath[i]);
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
            dest.w = w;
            dest.h = h;
            dest.x = (int)(ghost_x[i]) * w + 2*w;
            dest.y = (int)(ghost_y[i]) * h + 2*h;
            SDL_RenderCopy(gRenderer, tex, NULL, &dest);
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;
            SDL_DestroyTexture(tex);
            tex = NULL;
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(FrameTargetTime / 2);
    }
    DestroyReplayMusic();

    fclose(fmap);
    fclose(fpacman);
    fclose(fghosts);
}