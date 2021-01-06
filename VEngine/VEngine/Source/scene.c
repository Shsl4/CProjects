#include <VEngine/Headers/scene.h>
#include <VEngine/Headers/renderer.h>
#include <VEngine/Headers/scene.h>
#include <VEngine/Headers/camera.h>
#include <VEngine/Headers/physics.h>
#include <VEngine/Headers/VEngine.h>

int Scene_doubleCapacity(Scene *scene);

Scene *Scene_new(Renderer *renderer)
{
    Camera *camera = NULL;
    Scene *scene = NULL;
    int capacity = 1 << 10;

    int width  = Renderer_getWidth(renderer);
    int height = Renderer_getHeight(renderer);

    scene = (Scene *)calloc(1, sizeof(Scene));
    if (!scene) goto ERROR_LABEL;

    camera = Camera_new(width, height);
    if (!camera) goto ERROR_LABEL;

    scene->m_balls = (Ball *)calloc(capacity, sizeof(Ball));
    if (!scene->m_balls) goto ERROR_LABEL;

    scene->m_camera = camera;
    scene->m_renderer = renderer;
    scene->m_nbBalls = 0;
    scene->m_capacity = capacity;

    return scene;

ERROR_LABEL:
    printf("ERROR - Scene_new()\n");
    return NULL;
}

void Scene_free(Scene *scene)
{
    if (!scene) return;

    Camera_free(scene->m_camera);

    if (scene->m_balls)
    {
        free(scene->m_balls);
    }

    memset(scene, 0, sizeof(Scene));
    free(scene);
}

Renderer *Scene_getRenderer(Scene *scene)
{
    return scene->m_renderer;
}

Camera *Scene_getCamera(Scene *scene)
{
    return scene->m_camera;
}

int Scene_doubleCapacity(Scene *scene)
{
    Ball *newBalls = NULL;
    int newCapacity = scene->m_capacity << 1;

    newBalls = (Ball *)realloc(scene->m_balls, newCapacity * sizeof(Ball));
    if (!newBalls) goto ERROR_LABEL;

    scene->m_balls = newBalls;
    scene->m_capacity = newCapacity;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Scene_doubleCapacity()\n");
    return EXIT_FAILURE;
}

Ball *Scene_addBall(Scene *scene, Vec2 position)
{
    Ball *ball = NULL;

    if (scene->m_nbBalls >= scene->m_capacity)
    {
        int exitStatus = Scene_doubleCapacity(scene);
        if (exitStatus == EXIT_FAILURE) goto ERROR_LABEL;
    }

    ball = &scene->m_balls[scene->m_nbBalls];
    scene->m_nbBalls++;

    *ball = Ball_set(position);

    return ball;

ERROR_LABEL:
    printf("ERROR - Scene_addBall()\n");
    return NULL;
}

void Scene_removeBall(Scene *scene, Ball *ball)
{
    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);
    int index = (int)(ball - balls);
    int nbSprings = 0;

    if (index < 0 || index >= nbBalls)
        return;

    // Remove the springs
    nbSprings = ball->nbSprings;
    for (int i = 0; i < nbSprings; ++i)
    {
        Ball_deconnect(ball, ball->springs[i].other);
    }

    Ball *lastBall = &balls[nbBalls - 1];
    if (ball != lastBall)
    {
        // Copy the last ball
        *ball = *lastBall;

        // Update the springs
        ball->nbSprings = 0;
        nbSprings = lastBall->nbSprings;
        for (int i = 0; i < nbSprings; ++i)
        {
            Ball *other = lastBall->springs[i].other;
            float length = lastBall->springs[i].length;
            Ball_deconnect(lastBall, other);
            Ball_connect(ball, other, length);
        }
    }

    // Decrease the number of balls
    scene->m_nbBalls--;
}

int Scene_getNbBalls(Scene *scene)
{
    return scene->m_nbBalls;
}

Ball *Scene_getBalls(Scene *scene)
{
    return scene->m_balls;
}

BallQuery makeQuery(Ball* ball, float distance){

    BallQuery query = { .ball = ball, .distance = distance};
    return query;

}

void insertionSort(BallQuery* array, int size){

    for (int i = 1; i < size; i++) {

        BallQuery val = array[i];
        int j;

        for (j = i - 1; j >= 0; j--) {

            if(array[j].distance > val.distance){
                array[j + 1] = array[j];
            }
            else{
                break;
            }
        }
        array[j + 1] = val;
    }

}

int Scene_getNearestBallDynamic(Scene* scene, Vec2 position, float distanceThreshold, BallQuery** queries, int minQueries, int maxQueries, int* numBallsFound){

    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);

    BallQuery localQueries[nbBalls];

    int closeBalls = 0;

    for (int i = 0; i < nbBalls; ++i)
    {
        localQueries[i] = makeQuery(&balls[i], Vec2_distance(balls[i].position, position));
        if(localQueries[i].distance <= distanceThreshold) { ++closeBalls; }
    }

    if(minQueries > closeBalls){

        *numBallsFound = 0;
        *queries = NULL;
        return EXIT_FAILURE;

    }

    insertionSort(localQueries, nbBalls);

    closeBalls = closeBalls > maxQueries ? maxQueries : closeBalls;
    *numBallsFound = closeBalls;

    BallQuery* finalQueries = calloc(closeBalls, sizeof(BallQuery));

    for(int i = 0; i < closeBalls; ++i){
        finalQueries[i] = localQueries[i];
    }

    *queries = finalQueries;

    return EXIT_SUCCESS;

}


BallQuery Scene_getNearestBall(Scene *scene, Vec2 position) {

    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);

    BallQuery nearestBall = makeQuery(NULL, -1.0f);

    for (int i = 0; i < nbBalls; ++i) {

        float dist = Vec2_distance(position, balls[i].position);

        if(nearestBall.ball == NULL || nearestBall.distance > dist){
            nearestBall = makeQuery(&balls[i], dist);
        }

    }

    return nearestBall;

}

int Scene_getNearestBalls(Scene *scene, Vec2 position, BallQuery *queries, int nbQueries)
{
    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);

    BallQuery distances[nbBalls];

    for (int i=0; i < nbBalls; ++i)
    {
        distances[i] = makeQuery(&balls[i], Vec2_distance(balls[i].position, position));
    }

    insertionSort(distances, nbBalls);

    for(int i = 0; i < nbQueries; ++i){
        if(i >= nbBalls) { distances[i] = makeQuery(NULL, 0); continue; }
        queries[i] = distances[i];
    }

    return EXIT_SUCCESS;
}

void Scene_updateBalls(Scene *scene, float timeStep)
{
    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);

    for (int i = 0; i < nbBalls; i++)
    {
        Ball_updateVelocity(&balls[i], timeStep);
    }
    for (int i = 0; i < nbBalls; i++)
    {
        Ball_updatePosition(&balls[i], timeStep);
    }
}

void Scene_renderBalls(Scene *scene)
{
    int nbBalls = Scene_getNbBalls(scene);
    Ball *balls = Scene_getBalls(scene);
    Camera *camera = Scene_getCamera(scene);
    Renderer *renderer = Scene_getRenderer(scene);

    for (int i = 0; i < nbBalls; i++)
    {
        Ball *ball = &balls[i];
        int nbSprings = ball->nbSprings;
        int x1, y1;
        Camera_worldToView(camera, ball->position, &x1, &y1);

        for (int j = 0; j < nbSprings; j++)
        {
            Ball *other = ball->springs[j].other;
            int x2, y2;

            Camera_worldToView(camera, other->position, &x2, &y2);
            Renderer_drawLine(renderer, x1, y1, x2, y2, makeColor(255, 255, 255, 255));
        }

        Renderer_drawPoint(renderer, x1, y1, makeColor(255, 0, 255, 255));
    }
}

