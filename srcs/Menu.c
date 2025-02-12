menuOption *CreatMenuOption(int name, char pathOff[MaxLen], char pathOn[MaxLen])
{
    menuOption *new = (menuOption *) malloc(sizeof(menuOption));
    if(new == NULL)
        printf("Failed to creat new menu Option : %d !\n", name);

    new -> name = name;

    new -> offImg = IMG_Load(pathOff);
    if(new -> offImg == NULL)
        printf("Failed to Upload offImg Option %d, path : %s ! SDL Error : %s\n", name, pathOff, SDL_GetError());
    new -> onImg = IMG_Load(pathOn);
    if(new -> offImg == NULL)
        printf("Failed to Upload onImg Option number %d, path : %s ! SDL Error : %s\n", name, pathOn, SDL_GetError());

    new -> isOn = false;
    new -> pre = NULL;
    new -> nxt = NULL;

    return new;
}

menuOption *headOfMenuOption = NULL;

void InitMenu()
{
    menuOption *currentOption = NULL;
    int names[numOfMenuOptions] = {ENTER_NICKNAME, RECORDS, REPLAY, EXIT, REPLAY}; // must be updated!
    char paths[numOfMenuOptions][2][MaxLen] = {"../images/menu/PlayOff.png",
                               "../images/menu/PlayOn.png",
                               "../images/menu/RecordsOff.png",
                               "../images/menu/RecordsOn.png",
                               "../images/menu/ReplayOff.png",
                               "../images/menu/ReplayOn.png",
                               "../images/menu/ExitOff.png",
                               "../images/menu/ExitOn.png",
                               "../images/menu/RecordsOff.png",
                               "../images/menu/RecordsOn.png"};

    for(int i = 0; i < numOfMenuOptions; i++)
    {
        menuOption *newOption = CreatMenuOption(names[i], paths[i][0], paths[i][1]);
        newOption -> pre = currentOption;
        if(currentOption != NULL)
            currentOption -> nxt = newOption;
        currentOption = newOption;
        if(headOfMenuOption == NULL)
            headOfMenuOption = newOption;

    }

}

void UpdateMenu()
{
    static menuOption *currentChosenMenuOption = NULL;
    if(currentChosenMenuOption == NULL)
        currentChosenMenuOption = headOfMenuOption;
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
            gameStatus = currentChosenMenuOption -> name;
            if(gameStatus == ENTER_NICKNAME)
                strcpy(nickName, "");
            currentChosenMenuOption = headOfMenuOption;
            DestroyMenuMusic();
            break;
        default:
            break;
    }
    currentChosenMenuOption -> isOn = true;
    return;
}

void RenderMenu()
{
    //-------------
    SDL_Surface * loadedSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    float height = (float)screenWidth / (float)(numOfMenuOptions + 4);
    height /= 2;

    // loading menu pic
    loadedSurface = IMG_Load("../images/menu/menu.png");
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "../images/menu/menu.png", IMG_GetError() );
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

    //loading menu options:
    menuOption *currentOption = headOfMenuOption;
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

    PlayMenuMusic();
    SDL_RenderPresent(gRenderer);
}

void ClearMenu()
{
    menuOption *currentOption = headOfMenuOption;
    while(currentOption != NULL)
    {
        menuOption *next = currentOption -> nxt;
        SDL_FreeSurface(currentOption -> offImg);
        SDL_FreeSurface(currentOption -> onImg);
        free(currentOption);
        currentOption = next;
    }
}