// libraries
#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

//header files
#include "srcs/enums.h"
#include "srcs/consts.h"
#include "srcs/structs.h"
#include "srcs/maps.h"
#include "srcs/gameloopGvars.h"


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
TTF_Font *font = NULL, *font2 = NULL;
int gameStatus = START_SCREEN, lastPressedKey = NONE;
int lastFrameTime = 0;
float deltaTime; // delta time is a pre-meter that helps to manage moves per second (not per frame)
bool quit = false;

// c source files
#include "srcs/SoundEffects.c"
#include "srcs/StartScreen.c"
#include "srcs/EnterNickname.c"
#include "srcs/Menu.c"
#include "srcs/Records.c"
#include "srcs/ShortestPath.c"
#include "srcs/Replay.c"
#include "srcs/GameInit.c"
#include "srcs/GameUpdates.c"
#include "srcs/GameLoop.c"
#include "srcs/ChoseDifficulty.c"
#include "srcs/GameOver.c"


bool Init();
void Close();
void SetUp();
void ProcessInput();
void Update();
void Render();

bool Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    gWindow = SDL_CreateWindow("PacmanEdition25", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if(gWindow == NULL)
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    int imgFlag = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlag) & imgFlag))
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    imgFlag = IMG_INIT_JPG;
    if(!(IMG_Init(imgFlag) & imgFlag))
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    if (TTF_Init() < 0)
    {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont("../fonts/SquadaOne-Regular.ttf", 100);
    if(!font)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        return false;
    }

    font2 = TTF_OpenFont("../fonts/SquadaOne-Regular.ttf", 28);
    if(!font2)
    {
        printf("Error loading font2: %s\n", TTF_GetError());
        return false;
    }
    SDL_StartTextInput();

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    return true;
}

void Close()
{
    ClearMenu();
    ClearChoseDifficulty();
    CLearGameLoop();
    DestroyAllSoundEffects();
    Mix_CloseAudio();
    SDL_StopTextInput();
    TTF_CloseFont(font2);
    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SetUp()
{
    InitMenu();
    InitChoseDifficulty();
}

void ProcessInput()
{



    deltaTime = (float)(SDL_GetTicks() - lastFrameTime) / 1000.0f;
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        if(gameStatus != MENU) {
                            DestroyAllSoundEffects();
                            gameStatus = MENU;
                        }
                        else {
                            quit = true;
                        }
                        break;
                    case SDLK_UP:
                        lastPressedKey = UP;
                        break;
                    case SDLK_DOWN:
                        lastPressedKey = DOWN;
                        break;
                    case SDLK_RIGHT:
                        lastPressedKey = RIGHT;
                        break;
                    case SDLK_LEFT:
                        lastPressedKey = LEFT;
                        break;
                    case SDLK_RETURN:
                        if(gameStatus == ENTER_NICKNAME)
                        {
                            gameStatus = CHOSE_DIFFICULTY;
                            break;
                        }
                        lastPressedKey = ENTER;
                        break;
                    case SDLK_BACKSPACE:
                        if(gameStatus == ENTER_NICKNAME) {
                            if (strlen(nickName) > 0) {
                                int len = strlen(nickName);
                                nickName[len] = ' ';
                                nickName[len - 1] = '\0';
                            }
                        }
                        break;
                }
                break;
            case SDL_KEYUP:
                lastPressedKey = NONE;
                break;
            case SDL_TEXTINPUT:
                if(gameStatus == ENTER_NICKNAME && SDL_IsTextInputActive())
                {
                    strcat(nickName, e.text.text);
                }
                break;

            default:
                lastPressedKey = NONE;
                break;

        }
    }

    return;
}

void Update()
{
    if(gameStatus != GAME_LOOP)
    {
        int timeToWait = 100 - (SDL_GetTicks() - lastFrameTime);
        if(timeToWait > 0 && timeToWait <= 100)
            SDL_Delay(timeToWait);
    }
    else
    {
        int timeToWait = FrameTargetTime - (SDL_GetTicks() - lastFrameTime);
        if(timeToWait > 0 && timeToWait <= FrameTargetTime)
            SDL_Delay(timeToWait);
    }
    switch (gameStatus)
    {
        case START_SCREEN:
            StartScreenInit();
            break;
        case MENU:
            UpdateMenu();
            break;
        case EXIT:
            quit = true;
            break;
        case ENTER_NICKNAME:
            break;
        case CHOSE_DIFFICULTY:
            UpdateChoseDifficulty();
            break;
        case GAME_LOOP:
            UpdateGameLoop();
            break;
        default:
            break;
    }
    lastFrameTime = SDL_GetTicks();
    return;
}

void Render()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    switch (gameStatus)
    {
        case START_SCREEN:
            RenderStartScreen();
            gameStatus = MENU;
            break;
        case MENU:
            RenderMenu();
            break;
        case RECORDS:
            RenderRecords();
            break;
        case ENTER_NICKNAME:
            RenderEnterNickname();
            break;
        case CHOSE_DIFFICULTY:
            RenderChoseDifficulty();
            break;
        case GAME_LOOP:
            RenderGameLoop();
            break;
        case GAME_OVER:
            RenderGameOver();
            SDL_Delay(3000);
            gameStatus = MENU;
            break;
        case REPLAY:
            ShowReplay();
            gameStatus = MENU;
            break;
        default:
            SDL_SetRenderDrawColor(gRenderer, 245, 40, 145, 0);
            SDL_RenderClear(gRenderer);
            SDL_RenderPresent(gRenderer);
            break;
    }

}

int main(int argc, char *args[])
{
    srand(time(NULL));

    if(!Init())
    {
        printf("Initialize Failed! SDL Error : %s\n", SDL_GetError());
        Close();
        return 1;
    }

    SetUp();


    while(!quit)
    {
        ProcessInput();
        Update();
        Render();
    }

    Close();
    return 0;
}