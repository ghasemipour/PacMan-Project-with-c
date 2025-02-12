Mix_Music *menuMusic = NULL;
Mix_Music *gameBackgroundMusic = NULL;
Mix_Music *eatGhostMusic = NULL;
Mix_Music *defaultMusic = NULL;
Mix_Music *replayMusic = NULL;

Mix_Chunk *deathSound = NULL;
Mix_Chunk *eatFruitSound = NULL;
Mix_Chunk *collectStarSound = NULL;

Mix_Music *LoadMusic(char path[MaxLen])
{
    Mix_Music *music = Mix_LoadMUS(path);
    if(!music)
    {
        printf("Failed to load music! %s : %s", path, Mix_GetError());
        return NULL;
    }
    return music;
}

Mix_Chunk *LoadSound(char path[MaxLen])
{
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if(!sound)
    {
        printf("Failed to load sound! %s : %s", path, Mix_GetError());
        return NULL;
    }
    return sound;
}

void DestroyMenuMusic();
void DestroyGameBackgroundMusic();
void DestroyEatGhostMusic();
void DestroyDefaultMusic();
void DestroyEatFruitSound();
void DestroyDeathSound();
void DestroyCollectStarSound();
void DestroyReplayMusic();

void DestroyGameMusicsAndSounds()
{
    DestroyEatGhostMusic();
    DestroyGameBackgroundMusic();
    DestroyEatFruitSound();
    DestroyDeathSound();
    DestroyCollectStarSound();
}

void DestroyAllSoundEffects()
{
    DestroyReplayMusic();
    DestroyMenuMusic();
    DestroyDefaultMusic();
    DestroyGameMusicsAndSounds();
}

void PlayDefaultMusic()
{
    if(defaultMusic == NULL)
    {
        defaultMusic = LoadMusic("../sounds/pacman_intermission.wav");
        if(deathSound == NULL)
        {
            printf("Failed to load music! %s : %s", "../sounds/pacman_intermission.wav", SDL_GetError());
            return;
        }
    }

    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(defaultMusic, -1);
    }
    return;
}

void DestroyDefaultMusic()
{
    if(defaultMusic != NULL)
    {
        Mix_FreeMusic(defaultMusic);
        defaultMusic = NULL;
    }

    return;
}

void PlayReplayMusic()
{
    if(replayMusic == NULL)
    {
        replayMusic = LoadMusic("../sounds/replay.mp3");
        if(replayMusic == NULL)
        {
            printf("Failed to load music! %s : %s", "../sounds/replay.mp3", SDL_GetError());
            return;
        }
    }
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(replayMusic, -1);
    }
    return;
}

void DestroyReplayMusic()
{
    if(replayMusic != NULL)
    {
        Mix_FreeMusic(replayMusic);
        replayMusic = NULL;
    }
    return;
}

void PlayMenuMusic()
{
    if(menuMusic == NULL)
    {
        menuMusic = LoadMusic("../sounds/pacman_beginning.wav");
        if(menuMusic == NULL)
        {
            printf("Failed to load music! %s : %s", "../sounds/pacman_beginning.wav", SDL_GetError());
            return;
        }
    }
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(menuMusic, -1);
    }
}

void DestroyMenuMusic()
{
    if(menuMusic != NULL)
    {
        Mix_FreeMusic(menuMusic);
        menuMusic = NULL;
    }
}

void PlayGameBackgroundMusic()
{
    if(gameBackgroundMusic == NULL)
    {
        gameBackgroundMusic = LoadMusic("../sounds/pacman_chomp.wav");
        if(gameBackgroundMusic == NULL)
        {
            printf("Failed to load music! %s : %s", "../sounds/pacman_chomp.wav", SDL_GetError());
            return;
        }
    }

    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(gameBackgroundMusic, -1);
    }
}

void DestroyGameBackgroundMusic()
{
    if(gameBackgroundMusic != NULL)
    {
        Mix_FreeMusic(gameBackgroundMusic);
        gameBackgroundMusic = NULL;
    }

    return;
}

void PlayEatGhostMusic()
{
    if(eatGhostMusic == NULL)
    {
        eatGhostMusic = LoadMusic("../sounds/pacman_eatghost.wav");
        if(eatGhostMusic == NULL)
        {
            printf("Failed to load music! %s : %s", "../sounds/pacman_eatghost.wav", SDL_GetError());
            return;
        }
    }

    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(eatGhostMusic, -1);
    }
    return;
}

void DestroyEatGhostMusic()
{
    if(eatGhostMusic != NULL)
    {
        Mix_FreeMusic(eatGhostMusic);
        eatGhostMusic = NULL;
    }
    return;
}

void PlayEatFruitSound()
{
    if(eatFruitSound == NULL)
    {
        eatFruitSound = LoadSound("../sounds/pacman_eatfruit.wav");
        if(eatFruitSound == NULL)
        {
            printf("Failed to load sound! %s : %s", "../sounds/pacman_eatfruit.wav", SDL_GetError());
            return;
        }
        Mix_VolumeChunk(eatFruitSound, 120);
    }
    Mix_FadeInChannel(-1, eatFruitSound, 0, 0);
}

void DestroyEatFruitSound()
{
    if(eatFruitSound != NULL)
    {
        Mix_FreeChunk(eatFruitSound);
        eatFruitSound = NULL;
    }

    return;
}

void PlayDeathSound()
{
    if(deathSound == NULL)
    {
        deathSound = LoadSound("../sounds/pacman_death.wav");
        if(deathSound == NULL)
        {
            printf("Failed to load sound! %s : %s", "../sounds/pacman_death.wav", SDL_GetError());
            return;
        }
        Mix_VolumeChunk(deathSound, 60);
    }
    Mix_FadeInChannel(-1, deathSound, 0, 0);
}

void DestroyDeathSound()
{
    if(deathSound != NULL)
    {
        Mix_FreeChunk(deathSound);
        deathSound = NULL;
    }
    return;
}

void PlayCollectStarSound()
{
    if(collectStarSound == NULL)
    {
        collectStarSound = LoadSound("../sounds/collectStar.wav");
        if(collectStarSound == NULL)
        {
            printf("Failed to load sound! %s : %s", "../sounds/collectStar.wav", SDL_GetError());
            return;
        }
        Mix_VolumeChunk(collectStarSound, 120);
    }
    Mix_FadeInChannel(-1, collectStarSound, 0, 0);
}

void DestroyCollectStarSound()
{
    if(collectStarSound != NULL)
    {
        Mix_FreeChunk(collectStarSound);
        collectStarSound = NULL;
    }
    return;
}

