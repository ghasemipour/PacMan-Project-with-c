menuOption *headOfDifficultyOptions = NULL;

void InitChoseDifficulty()
{
    menuOption *currentOption = NULL;
    int names[3] = {EASY, NORMAL, HARD};
    char paths[3][2][MaxLen] = {
            "../images/difficulty/EasyOff.png",
            "../images/difficulty/EasyOn.png",
            "../images/difficulty/NormalOff.png",
            "../images/difficulty/NormalOn.png",
            "../images/difficulty/HardOff.png",
            "../images/difficulty/HardOn.png"
    };
    for(int i = 0; i < 3; i++)
    {
        menuOption *newOption = CreatMenuOption(names[i], paths[i][0], paths[i][1]);
        newOption -> pre = currentOption;
        if(currentOption != NULL)
            currentOption -> nxt = newOption;
        currentOption = newOption;
        if(headOfDifficultyOptions == NULL)
            headOfDifficultyOptions = newOption;
    }
}

void UpdateChoseDifficulty()
{
    static menuOption *currentChosenMenuOption = NULL;
    if(currentChosenMenuOption == NULL)
        currentChosenMenuOption = headOfDifficultyOptions;
    currentChosenMenuOption -> isOn = false;
    switch (lastPressedKey) {
        case UP:
            if(currentChosenMenuOption->pre != NULL)
                currentChosenMenuOption = currentChosenMenuOption->pre;
            break;
        case DOWN:
            if(currentChosenMenuOption->nxt != NULL)
                currentChosenMenuOption = currentChosenMenuOption->nxt;
            break;
        case ENTER:
            DestroyDefaultMusic();
            InitGameLoop(currentChosenMenuOption->name);
            gameStatus = GAME_LOOP;
            currentChosenMenuOption = headOfDifficultyOptions;
            break;
        default:
            break;
    }
    currentChosenMenuOption -> isOn = true;
    return;
}

void RenderChoseDifficulty()
{
    //-------------
    SDL_Surface * loadedSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    float height = (float)screenWidth / (float)(7);
    height /= 2;

    // loading difficulty pic
    loadedSurface = IMG_Load("../images/difficulty/Difficulty.png");
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "../images/difficulty/Difficulty.png", IMG_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL,  &dest.w, &dest.h);
    dest.w = screenWidth / 2;
    dest.h = height * 2;
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = height;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    //loading difficulty options:
    menuOption *currentOption = headOfDifficultyOptions;
    dest.y = height * 3;
    while(currentOption != NULL)
    {
        if(currentOption -> isOn)
            tex = SDL_CreateTextureFromSurface(gRenderer, currentOption->onImg);
        else
            tex = SDL_CreateTextureFromSurface(gRenderer, currentOption->offImg);

        SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
        dest.w = screenWidth / 2;
        dest.h = height * 2;
        dest.x = (screenWidth - dest.w) / 2;
        SDL_RenderCopy(gRenderer, tex, NULL, &dest);
        SDL_DestroyTexture(tex);
        tex = NULL;

        dest.y += height * (1.5);
        currentOption = currentOption -> nxt;

    }

    //loading pacman pic:
    loadedSurface = IMG_Load("../images/menu/PacmanPic.jpg");
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = screenWidth / 2;
    dest.h = height * (3);
    dest.x = (screenWidth - dest.w) / 2;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    SDL_RenderPresent(gRenderer);
}

void ClearChoseDifficulty()
{
    menuOption *currentOption = headOfDifficultyOptions;
    while(currentOption != NULL)
    {
        menuOption *next = currentOption -> nxt;
        SDL_FreeSurface(currentOption -> onImg);
        SDL_FreeSurface(currentOption -> offImg);
        free(currentOption);
        currentOption = next;
    }
}