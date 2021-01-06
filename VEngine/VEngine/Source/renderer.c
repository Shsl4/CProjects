#include <VEngine/Headers/renderer.h>
#include <VEngine/Headers/vector.h>
#include <VEngine/Headers/VEngine.h>
#include <VEngine/Headers/window.h>

SDL_Color White = {255, 255, 255};

int Renderer_getWidth(Renderer *renderer)
{
    return renderer->m_width;
}

int Renderer_getHeight(Renderer *renderer)
{
    return renderer->m_height;
}

void Renderer_clear(Renderer* renderer)
{
    SDL_Renderer* rendererSDL = renderer->m_rendererSDL;
    SDL_SetRenderDrawColor(rendererSDL, (Uint8)25, (Uint8)25, (Uint8)25, (Uint8)255);
    SDL_RenderClear(rendererSDL);
}

void Renderer_fill(Renderer *renderer, Color color)
{
    SDL_Rect rect;
    SDL_Renderer* rendererSDL = renderer->m_rendererSDL;

    rect.x = 0;
    rect.y = 0;
    rect.w = renderer->m_width;
    rect.h = renderer->m_height;

    SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rendererSDL, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, (Uint8)color.a);
    SDL_RenderFillRect(rendererSDL, &rect);
}

void Renderer_drawPoint(Renderer *renderer, int x, int y, Color color)
{
    SDL_Renderer* rendererSDL = renderer->m_rendererSDL;

    SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rendererSDL, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, (Uint8)color.a);
    SDL_RenderDrawLine(rendererSDL, x - 5, y, x + 5, y);
    SDL_RenderDrawLine(rendererSDL, x, y - 5, x, y + 5);
}

void Renderer_drawLine(Renderer *renderer, int x1, int y1, int x2, int y2, Color color)
{
    SDL_Renderer* rendererSDL = renderer->m_rendererSDL;

    SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rendererSDL, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, (Uint8)color.a);
    SDL_RenderDrawLine(rendererSDL, x1, y1, x2, y2);
}

Color makeColor(int r, int g, int b, int a)
{
    Color color;
    color.r = (uint8_t)r;
    color.g = (uint8_t)g;
    color.b = (uint8_t)b;
    color.a = (uint8_t)a;

    return color;
}

void Renderer_update(Renderer* renderer)
{
    SDL_RenderPresent(renderer->m_rendererSDL);
}

int randInRange(int min, int max){
    return rand() % ( max - min + 1 ) + min;
}

Color randomColor() {
    Color result = { .r = randInRange(100, 255),
                     .g = randInRange(100, 255),
                     .b = randInRange(100, 255),
                     .a = 255};
    return result;
}

void drawText(SDL_Renderer* renderer, Vec2 pos, TTF_Font* font, char *text, SDL_Color color, bool freeText) {

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect = { .x = pos.x, .y = pos.y, .w = strlen(text) * 10.0f, .h = TEXT_DRAW_HEIGHT};
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    if(freeText){

        free(text);

    }

}


void drawText_FromEnd(SDL_Renderer *renderer, Vec2 pos, TTF_Font *font, char *text, SDL_Color color, bool freeText) {

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect = { .x = Renderer_getWidth(getRenderer()) - strlen(text) * 10.0f - pos.x, .y = pos.y, .w = strlen(text) * 10.0f, .h = TEXT_DRAW_HEIGHT};
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    if(freeText){

        free(text);

    }

}
