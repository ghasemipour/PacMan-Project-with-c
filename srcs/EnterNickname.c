char nickName[MaxLen];

void RenderEnterNickname()
{
    SDL_Surface *loadedSurface = NULL;
    SDL_Surface *textSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    loadedSurface = IMG_Load("../images/nickname/EnterNickname.png");
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "../images/nickname/EnterNickname.png", IMG_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = screenWidth / (1.5);
    dest.h = screenHeight / 3;
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = 0;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;

    SDL_Color color = {225, 225, 225, 225};
    int len = strlen(nickName);
 //   textSurface = TTF_RenderText_Solid(font, "Hello world", color);
    if(len > 0)
    {
        textSurface = TTF_RenderText_Solid(font, nickName, color);
    }
    else
    {
        textSurface = TTF_RenderText_Solid(font, " ", color);
    }

    if(!textSurface)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", " FUCKING Nickname", TTF_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = (screenWidth - dest.w) / 2;
    dest.y += (screenHeight / 3) + (screenHeight / 6);
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
    tex = NULL;
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    PlayDefaultMusic();
    SDL_RenderPresent(gRenderer);
}