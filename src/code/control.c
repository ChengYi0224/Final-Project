/*
    程式包含以下功能:
    -   滑鼠 / 鍵盤輸入(取消，移至主程式或是其他function實現)
    -   介面顯示(取消，移至主程式或是其他function實現)
        -   台詞顯示
        -   背景顯示
        -   頭像顯示
        -   特效
    -   從script.toml讀取劇本
        1.  讀取劇本
        2.  分類物件
        3.  依序執行(取消，移至主程式或是其他function實現)
*/
#include "control.h"
int64_t gGameVolume = 100;
TTF_Font *gFontDefault = NULL;
SDL_Color gColorWHITE = {255, 255, 255, 255};
SDL_Color gColorBLACK = {0, 0, 0, 255};
SDL_Color gColorGREY = {128, 128, 128, 255};
SDL_Color gColorLGREY = {192, 192, 192, 255};
SDL_Color gColorDGREY = {64, 64, 64, 255};
SDL_Color gColorRED = {255, 0, 0, 255};
SDL_Color gColorGREEN = {0, 255, 0, 255};
SDL_Color gColorBLUE = {0, 0, 255, 255};

SDL_Rect gRectDialogue = {360, 370, 850, 300};
SDL_Rect gRectBackground = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Rect gRectTachie = {70, 70, 270, 600};
SDL_Rect gRectText = {390, 410, 800, 235};
SDL_Rect gRectAvatar = {360 + 850 - 70, 370 + 300 - 70, 70, 70};

SDL_Rect gRectInventory[4] = {
    {940, 600, 80, 80},
    {1040, 600, 80, 80},
    {1140, 600, 80, 80},
    {1240, 600, 80, 80}};

toml_table_t *gRootTabGameSaveRead = NULL;

int8_t DisplayImg(SDL_Renderer *renderer, char *imgPath, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
    // 讀取圖片
    SDL_Surface *image = IMG_Load(imgPath);
    // 建立材質
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    // 顯示
    SDL_RenderCopy(renderer, texture, srcRect, dstRect);
    // 釋放資源
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

int8_t DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect)
{
    // 建立材質
    // SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, color, dstRect->w);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (texture == NULL)
    {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    dstRect->w = textSurface->w;
    if (dstRect->h > textSurface->h)
        dstRect->h = textSurface->h;
    SDL_RenderCopy(renderer, texture, NULL, dstRect);
    // SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);

    return 1;
}

int8_t DisplayUTF8(SDL_Renderer *renderer, uint8_t *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect)
{
    // SDL_RenderFillRect(renderer, dstRect);
    //  建立材質
    //  SDL_Surface *textSurface = TTF_RenderUTF8_Solid_Wrapped(font, text, color, dstRect->w);
    //  SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, dstRect->w);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (texture == NULL)
    {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    dstRect->w = textSurface->w;
    if (dstRect->h > textSurface->h)
        dstRect->h = textSurface->h;
    SDL_RenderCopy(renderer, texture, NULL, dstRect);
    // SDL_RenderPresent(renderer); // 由外面的呼叫者統一更新畫面以避免閃爍
    SDL_DestroyTexture(texture);

    return 1;
}

int8_t DisplayItem(SDL_Renderer *renderer, toml_table_t *item, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
    // SDL_Texture *texture = LoadTexture(renderer, item->string["image"]);
}

int8_t renderButton(SDL_Renderer *renderer, Button *button)
{
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    if (button->isHovered)
    {
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Highlight color
        SDL_RenderFillRect(renderer, &button->rect);
    }

    if (button->isClicked)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50); // Clicked color
        SDL_RenderFillRect(renderer, &button->rect);
    }
}

int8_t handleButton(SDL_Event *event, Button *button)
{
    int mouseX, mouseY;
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
            mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h)
        {
            printf("Button clicked!\\n");
            return 1;
        }
    }
    return 0; // 按鈕沒有被點擊
}

struct tm *getLocalTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return timeinfo;
}