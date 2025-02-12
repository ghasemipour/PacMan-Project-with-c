int numOfRecords;

int cmp(const record_t *a, const record_t *b)
{
    return b->score - a->score;
}

int cmpStr(const record_t *a, const record_t *b)
{
    if(strcmp(a->nickname, b->nickname) == 0)
    {
        return b->score - a->score;
    }
    return strcmp(a->nickname, b->nickname);
}

void AddRecord()
{
    FILE *fptr;
    fptr = fopen("../texts/NumOfRecords.txt", "r");
    fscanf(fptr, "%d", &numOfRecords);
    fclose(fptr);
    numOfRecords++;
    fptr = fopen("../texts/NumOfRecords.txt", "w");
    fprintf(fptr, "%d\n", numOfRecords);
    fflush(fptr);
    fclose(fptr);

    fptr = fopen("../texts/Records.txt", "a");
    fprintf(fptr, "%s\n", nickName);
    fprintf(fptr, "%d\n", score);
    fflush(fptr);
    fclose(fptr);
    return;
}

void RenderRecords()
{
    FILE *fptr;
    fptr = fopen("../texts/NumOfRecords.txt", "r");
    fscanf(fptr, "%d", &numOfRecords);
    fclose(fptr);
    record_t recordsList[numOfRecords];
    fptr = fopen("../texts/Records.txt", "r");
    if(fptr == NULL)
    {
        printf("Failed to open file\n");
        return;
    }
    for(int i = 0; i < numOfRecords; i++)
    {
        fgets(recordsList[i].nickname, MaxLen, fptr);
        recordsList[i].nickname[strcspn(recordsList[i].nickname, "\n")] = '\0';

        fscanf(fptr, "%d", &recordsList[i].score);
        fgetc(fptr);
    }
   // fflush(fptr);
    fclose(fptr);

    qsort(recordsList, numOfRecords, sizeof(record_t), cmpStr);
    for(int i = 1; i < numOfRecords; i++)
    {
        if(strcmp(recordsList[i].nickname, recordsList[i-1].nickname) == 0)
        {
            recordsList[i].score = -1;
        }
    }
    qsort(recordsList, numOfRecords, sizeof(record_t), cmp);

    //Rendering:
    char medalPath[3][MaxLen] = {
            "../images/records/goldenMedal.png",
            "../images/records/silverMedal.png",
            "../images/records/bronzeMedal.png"
    };
    SDL_Surface *loadedSurface = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect dest;
    SDL_Color color = {225, 225, 225, 225};
    SDL_Color colorMedal[3] = {{241,195,31,225}, {162,161,156,225}, {171,89,5,225}};

    char str[] = "SCOREBOARD";
    loadedSurface = TTF_RenderText_Solid(font, str, color);
    if(loadedSurface == NULL)
    {
        printf( "Unable to load text %s! SDL_image Error: %s\n", "last Record", TTF_GetError() );
        return;
    }
    tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.y = 0;
    dest.x = (screenWidth - dest.w) / 2;
    SDL_RenderCopy(gRenderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
    tex = NULL;
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;

    int newNumOfRecords = 0;
    fptr = fopen("../texts/Records.txt", "w");
    dest.y += dest.h;
    for(int i = 0; i < fmin(10, numOfRecords); i++)
    {
        if(recordsList[i].score < 0)
            break;
        newNumOfRecords++;
        fprintf(fptr, "%s\n", recordsList[i].nickname);
        fprintf(fptr, "%d\n", recordsList[i].score);

        char str_final[MaxLen];
        str_final[0] = '\0';
        char score_str[MaxLen];
        score_str[0] = '\0';
        sprintf(score_str, "%d", recordsList[i].score);
        printf("%s\n", score_str);
        if(i < 3)
        {
            strcpy(str_final, recordsList[i].nickname);
            int len = strlen(str_final);
            str_final[len] = ' ';
            str_final[48 - strlen(score_str)] = ' ';
            for(int j = len+1; j < 48 - strlen(score_str); j++)
                str_final[j] = '.';

            str_final[48-strlen(score_str)] = '\0';
            strcat(str_final, score_str);
            loadedSurface = TTF_RenderText_Solid(font2, str_final, colorMedal[i]);
            if(!loadedSurface)
            {
                printf( "Unable to load text %s! SDL_image Error: %s\n", "ranks", TTF_GetError() );
                return;
            }
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
            dest.x = (screenWidth - dest.w - dest.h) / 2 + dest.h;
            SDL_RenderCopy(gRenderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            tex = NULL;
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;

            loadedSurface = IMG_Load(medalPath[i]);
            if(!loadedSurface)
            {
                printf( "Unable to load picture %s! SDL_image Error: %s\n", medalPath[i], IMG_GetError() );
                //return;
            }
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            dest.x = (screenWidth - dest.w - dest.h) / 2;
            dest.w = dest.h;
            SDL_RenderCopy(gRenderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            tex = NULL;
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;

        }
        else
        {
            sprintf(str_final, "%d", i+1);
            int len = strlen(str_final);
            str_final[len] = '.';
            str_final[len+1] = '\0';
            strcat(str_final, recordsList[i].nickname);
            len = strlen(str_final);
            str_final[len] = ' ';
            str_final[48 - strlen(score_str)] = ' ';
            for(int j = len+1; j < 48 - strlen(score_str); j++)
                str_final[j] = '.';

            str_final[48-strlen(score_str)] = '\0';
            strcat(str_final, score_str);
            loadedSurface = TTF_RenderText_Solid(font2, str_final, color);
            if(!loadedSurface)
            {
                printf( "Unable to load text %s! SDL_image Error: %s\n", "ranks", TTF_GetError() );
                return;
            }
            tex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
            dest.x = (screenWidth - dest.w) / 2;
            SDL_RenderCopy(gRenderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            tex = NULL;
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;

        }
        dest.y += 2 * dest.h;

    }
    fflush(fptr);
    fclose(fptr);
    PlayDefaultMusic();
    SDL_RenderPresent(gRenderer);
    numOfRecords = newNumOfRecords;
    fptr = fopen("../texts/NumOfRecords.txt", "w");
    fprintf(fptr, "%d\n", numOfRecords);
    fflush(fptr);
    fclose(fptr);
    return;
}