void RenderGameOver()
{
    CLearGameLoop();

    SDL_Surface *loadedSurface = IMG_Load("../images/gameOver/gameOver.jpg");
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "../images/gameOver/gameOver.jpg", IMG_GetError() );
        return;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = screenWidth;
    dest.h = screenHeight;
    dest.x = 0;
    dest.y = 0;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
    tex = NULL;
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;

    char record[MaxLen];
    char score_str[MaxLen];
    sprintf(score_str, "%d", score);
    strcpy(record, nickName);
    int len = strlen(record);
    record[len] = ' ';
    record[50 - strlen((score_str)) - 2] = ' ';
    for(int i = len+1; i < 50 - strlen(score_str) - 2; i++)
        record[i] = '.';

    record[48 - strlen(score_str)] = '\0';
    strcat(record, score_str);

    SDL_Color color = {225,222,89,225};
    loadedSurface = TTF_RenderText_Solid(font, record, color);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load text %s! SDL_image Error: %s\n", "last Record", TTF_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = dest.w / 3;
    dest.h = dest.h / 3;
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = screenHeight / 2 + screenHeight / 6;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
    tex = NULL;
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;

    SDL_RenderPresent(gRenderer);
}