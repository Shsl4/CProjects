//
// Created by sl4sh on 1/5/21.
//

#include <VEngine/Headers/VEngine.h>
#include <VEngine/Headers/vector.h>
#include <VEngine/Headers/input.h>
#include <VEngine/Headers/window.h>
#include <VEngine/Headers/timer.h>
#include <time.h>

VEngineData engineData = { NULL, NULL};
TTF_Font* defaultFont = NULL;
Timer* timer;
Uint32 fps_lastTime = 0;
Uint32 fps_current = 0;
Uint32 fps_frames = 0;

bool runEngine = true;

void VEngine_Tick(void);
void VEngine_Play(void);
void VEngine_InitSDL(void);
void VEngine_Cleanup(void);
void VEngine_Kill();
void VEngine_drawFPS();

void VEngine(Game game) {

    VE_LOG("VEngine v0.1 initiating...", LOG_INFO);

    srand(time(NULL));

    engineData.game = &game;

    bool functionsDefined = false;
    functionsDefined |= (getEngine().game->tickFunction == NULL);
    functionsDefined |= (getEngine().game->playFunction == NULL);
    functionsDefined |= (getEngine().game->onQuitFunction == NULL);
    functionsDefined |= (getEngine().game->inputProcessFunction == NULL);

    if(functionsDefined){

        VE_LOG("The given game's tick, play, input or quit function is null or invalid.", LOG_FATAL);
        VEngine_Kill();

    }

    VEngine_InitSDL();

    SDL_ShowCursor(0);
    engineData.window = Window_new(WINDOW_WIDTH, WINDOW_HEIGHT);

    if (engineData.window == NULL) {

        VE_LOG("Failed to create a window. Reason: %s", LOG_FATAL, SDL_GetError());
        VEngine_Kill();

    }

    VE_LOG("Built window.", LOG_INFO);

    char* fontPath = "./Fonts/Espera-Regular.ttf";

    defaultFont = TTF_OpenFont(fontPath, 30);

    if(defaultFont == NULL){

        VE_LOG("Failed to load font. Make sure there is 'Espera-Regular.ttf' located at %s", LOG_FATAL, fontPath);
        VEngine_Kill();

    }

    VE_LOG("Loaded font.", LOG_INFO);

    timer = Timer_new();

    if(defaultFont == NULL){

        VE_LOG("Failed to create engine timer.", LOG_FATAL);
        VEngine_Kill();

    }

    char* gameName = getEngine().game->gameName == NULL ? "an unknown game" : getEngine().game->gameName;

    VE_LOG("Successfully initiated. Welcome! Now launching %s...", LOG_SUCCESS, gameName);

    VEngine_Play();
    VEngine_Tick();
    VEngine_Cleanup();

}

void VEngine_Log(char *format, LogSeverity severity, ...) {

    char* buf;

    va_list list;
    va_start(list, severity);

    switch (severity) {
        case 1:
            asprintf(&buf, "%sVEngine | (Success) : %s\n", TCOLOR_GREEN, format);
            break;
        case 2:
            asprintf(&buf, "%sVEngine | (Warn) : %s\n", TCOLOR_YELLOW, format);
            break;
        case 3:
            asprintf(&buf, "%sVEngine | (Err) : %s\n", TCOLOR_RED, format);
            break;
        case 4:
            asprintf(&buf, "%sVEngine | (FATAL) : %s\n", TCOLOR_RED, format);
            break;
        default:
            asprintf(&buf, "%sVEngine | (Info) : %s\n", TCOLOR_RESET, format);
            break;
    }

    vprintf(buf, list);
    va_end(list);

}

void VEngine_InitSDL(void) {

    if(SDL_Init(SDL_INIT_VIDEO) != 0) { VE_LOG("SDL failed to initiate. Reason: ", LOG_FATAL, SDL_GetError()); VEngine_Kill(); return; }

    VE_LOG("SDL initiated.", LOG_INFO);

    if(TTF_Init() == -1) { VE_LOG("SDL_TTF failed to initiate. Reason: ", LOG_FATAL, SDL_GetError()); VEngine_Kill(); return; }

    VE_LOG("SDL_TTF initiated.", LOG_INFO);

}

void VEngine_Tick(void) {

    void (*tickFunction)() = getEngine().game->tickFunction;

    while(runEngine){

        Timer_update(timer);

        Renderer_clear(getEngine().window->m_renderer);

        tickInput();

        if(!runEngine) { break;}

        (*tickFunction)();

        setMouseDelta(Vec2_set(0.0f, 0.0f));

        VEngine_drawFPS();

        Renderer_update(getEngine().window->m_renderer);

    }

}

void VEngine_Play(void) {

    Timer_start(timer);
    initInput();
    void (*playFunction)() = getEngine().game->playFunction;
    playFunction();

}

void VEngine_Cleanup(void) {

    VE_LOG("Cleaning up...", LOG_WARNING);

    TTF_CloseFont(defaultFont);
    Window_free(getWindow());
    Timer_free(timer);

    engineData.game = NULL;
    engineData.window = NULL;
    timer = NULL;

    TTF_Quit();
    SDL_Quit();

    VE_LOG("Goodbye!", LOG_INFO);

    exit(0);

}

void VEngine_Kill() {
    exit(-1);
}

void VEngine_drawFPS() {

    fps_frames++;
    if (fps_lastTime < SDL_GetTicks() - 1000)
    {
        fps_lastTime = SDL_GetTicks();
        fps_current = fps_frames;
        fps_frames = 0;
    }

    char* fpsText = NULL;
    asprintf(&fpsText, "FPS: %.2u", fps_current);
    drawText(getSDLRenderer(), Vec2_set(0.0f, 0.0f), defaultFont, fpsText, White, true);

}

void VEngine_Stop() {

    void (*quitFunction)() = getEngine().game->onQuitFunction;
    quitFunction();
    runEngine = false;

}

VEngineData getEngine(void) {
    return engineData;
}

float getDeltaTime() {
    return timer->m_delta;
}

