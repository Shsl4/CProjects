//
// Created by sl4sh on 1/5/21.
//

#include <DemoGame.h>
#include <VEngine/Headers/scene.h>
#include <VEngine/Headers/input.h>
#include <VEngine/Headers/window.h>
#include <VEngine/Headers/camera.h>
#include <VEngine/Headers/physics.h>
#include <VEngine/Headers/timer.h>

Camera *camera = NULL;
Scene *scene = NULL;

Vec2 mouseWorldPos = {.x = 0, .y = 0};

float accumulator = 0.0f;
float timeStep = 1.f / 100.f;
float maxHeight = 0.0f;

void renderGround();
void removeClosestBall();
void mouseInteraction();
void renderCursor();
void mouseGrab();

int mouseMode = 0;

int mouseClick = 0;

Game makeDemoGame() {
    Game game = { .gameName = "DemoGame", .playFunction = &play, .tickFunction = &tick, .onQuitFunction = &onQuit, .inputProcessFunction = &processInput};
    return game;
}

void play() {

    scene = Scene_new(getEngine().window->m_renderer);
    camera = Scene_getCamera(scene);

    Ball *ball1 = Scene_addBall(scene, Vec2_set(6.0f, 1.0f));
    Ball *ball2 = Scene_addBall(scene, Vec2_set(7.0f, 3.0f));
    Ball *ball3 = Scene_addBall(scene, Vec2_set(8.0f, 1.0f));

    Ball_connect(ball1, ball2, 1.0f);
    Ball_connect(ball1, ball3, 1.0f);
    Ball_connect(ball2, ball3, 1.0f);

}

void onQuit() {

    Scene_free(scene);

    camera = NULL;
    scene = NULL;

}

void tick() {

    renderGround();

    // Get the mouse position
    Camera_viewToWorld(camera, getMouseX(), getMouseY(), &mouseWorldPos);

    float delta = getDeltaTime();

    // Update the physics engine
    accumulator += delta;
    while (accumulator >= timeStep) {
        Scene_updateBalls(scene, timeStep);
        accumulator -= timeStep;
    }

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        // When the framerate is too high, the delta value is so low that a movement can barely be seen
        if(delta < 0.001f) delta = 0.001f;
        Camera_move(camera, Vec2_scale(Vec2_set(-getMouseDelta().x, getMouseDelta().y), 10.0f * delta));
    }

    renderCursor();

    if(mouseMode == 1){
        mouseGrab();
    }
    else{
        mouseInteraction();
    }

    renderGameText();

    Scene_renderBalls(scene);

}

void renderCursor() {
    int x1, y1;
    Camera_worldToView(camera, mouseWorldPos, &x1, &y1);
    Renderer_drawPoint(getRenderer(), x1, y1, makeColor(0, 255, 255, 255));
}

void mouseInteraction() {

    if (mouseWorldPos.y > 0.0f) {

        BallQuery *Queries;
        int foundBalls = 0;
        Scene_getNearestBallDynamic(scene, mouseWorldPos, 2.0f, &Queries, 2, 3, &foundBalls);

        for (int i = 0; i < foundBalls; ++i) {

            int x, mx, y, my;
            Camera_worldToView(camera, mouseWorldPos, &mx, &my);
            Camera_worldToView(camera, Queries[i].ball->position, &x, &y);
            Renderer_drawLine(getRenderer(), mx, my, x, y, makeColor(255, 255, 0, 255));

        }

        if (mouseClick) {

            if (foundBalls > 0) {

                if (maxHeight < mouseWorldPos.y) {
                    maxHeight = mouseWorldPos.y;
                }

                Ball *ball = Scene_addBall(scene, mouseWorldPos);

                for (int i = 0; i < foundBalls; ++i) {

                    Ball_connect(ball, Queries[i].ball, Vec2_distance(mouseWorldPos, Queries[i].ball->position));

                }
            }

            mouseClick = 0;

        }

        free(Queries);

    }

}

void removeClosestBall() {
    BallQuery query = Scene_getNearestBall(scene, mouseWorldPos);

    if (query.distance <= 2.0f) {
        Scene_removeBall(scene, query.ball);
    }
}

void renderGameText() {

    char *maxHeightText = NULL;
    asprintf(&maxHeightText, "Max Height: %.2fm", maxHeight);

    char *placesBallsText = NULL;
    asprintf(&placesBallsText, "Placed Balls: %d", Scene_getNbBalls(scene));

    char *mouseModeText = NULL;
    asprintf(&mouseModeText, "Mouse mode: %s. Press 'F' to toggle.", mouseMode ? "Grab" : "Link");

    drawText(getSDLRenderer(), Vec2_set(0.0f, 22.5f), defaultFont, maxHeightText, White, true);
    drawText(getSDLRenderer(), Vec2_set(0.0f, 45.0f), defaultFont, placesBallsText, White, true);
    drawText_FromEnd(getSDLRenderer(), Vec2_set(0.0f, 0.0f), defaultFont, mouseModeText, White, true);

}

void renderGround() {

    int groundWorldToViewX = 0;
    int groundWorldToViewY = 0;
    Camera_worldToView(camera, Vec2_set(0.0f, 0.0f), &groundWorldToViewX, &groundWorldToViewY);

    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(getSDLRenderer(), 200, 200, 200, 255);
    SDL_Rect rect = { .x = -1000000, .y = groundWorldToViewY, .h = 12, .w = 2000000 };
    SDL_RenderFillRect(getSDLRenderer(), &rect);

}

void mouseGrab() {

    if (mouseWorldPos.y > 0.0f) {

        BallQuery *Queries;
        int foundBalls = 0;
        Scene_getNearestBallDynamic(scene, mouseWorldPos, 1.0f, &Queries, 1, 1, &foundBalls);

        if(foundBalls == 1){

            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){

                Queries[0].ball->position = mouseWorldPos;
                Queries[0].ball->velocity = Vec2_set(0.0f, 0.0f);

            }
            else{

                int x, mx, y, my;
                Camera_worldToView(camera, mouseWorldPos, &mx, &my);
                Camera_worldToView(camera, Queries[0].ball->position, &x, &y);
                Renderer_drawLine(getRenderer(), mx, my, x, y, makeColor(0, 255, 0, 255));

            }

        }

    }

}

void processInput(SDL_Event evt) {

    SDL_MouseButtonEvent mouseButton;

    switch (evt.type) {
        case SDL_KEYDOWN:
            if(evt.key.keysym.sym == 'f'){
                mouseMode = !mouseMode;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:

            mouseButton = evt.button;

            if (mouseButton.button == SDL_BUTTON_LEFT)
                mouseClick = 1;

            if (mouseButton.button == SDL_BUTTON_RIGHT) {
                removeClosestBall();
            }
            break;
    }

}


