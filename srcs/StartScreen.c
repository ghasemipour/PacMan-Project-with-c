void StartScreenInit()
{
    char path[] = "../images/StartPic.png";
    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
        return;
    }
    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void RenderStartScreen()
{
    SDL_Rect dest;
    SDL_QueryTexture(gTexture, NULL, NULL, &dest.w, &dest.h);
    dest.x = (screenWidth - dest.w) / 2;
    dest.y = (screenHeight - dest.h) / 2;
    float yPos = (float)screenHeight;
    while(dest.y >= (screenHeight - dest.h) / 2)
    {
        SDL_RenderClear(gRenderer);
        dest.y = (int)yPos;
        SDL_RenderCopy(gRenderer, gTexture, NULL, &dest);
        SDL_RenderPresent(gRenderer);
        yPos -= (float) ScrollSpeed / FPS;
        SDL_Delay(FrameTargetTime);
    }
    SDL_Delay(1000);
    SDL_DestroyTexture(gTexture);

    gTexture = NULL;
    SDL_DestroyTexture(gTexture);
}