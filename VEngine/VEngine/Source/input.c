//
// Created by sl4sh on 1/5/21.
//

#include <VEngine/Headers/VEngine.h>
#include <VEngine/Headers/input.h>

int mouseX = 0;
int mouseY = 0;
Vec2 mouseDelta = {.x = 0, .y = 0};
void (*userInputFunction)(SDL_Event);

void initInput() {
    userInputFunction = getEngine().game->inputProcessFunction;
}

void tickInput()
{

    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {

        SDL_Scancode scanCode;

        switch (evt.type) {

            case SDL_QUIT:
                VEngine_Stop();
                return;

            case SDL_KEYDOWN:

                scanCode = evt.key.keysym.scancode;
                if (evt.key.repeat)
                    break;

                switch (scanCode) {
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_RETURN:
                        VEngine_Stop();
                        return;
                    default:
                        break;
                }

                break;

            case SDL_MOUSEMOTION:

                mouseX = evt.motion.x;
                mouseY = evt.motion.y;
                mouseDelta = Vec2_set(evt.motion.xrel, evt.motion.yrel);
                break;

            default:
                mouseDelta = Vec2_set(0.0f, 0.0f);
                break;

        }

        userInputFunction(evt);

    }

}

int getMouseX() { return mouseX; }

int getMouseY() { return mouseY; }

Vec2 getMouseDelta() { return mouseDelta; }

void setMouseDelta(Vec2 inputMouseDelta) { mouseDelta = inputMouseDelta; }
