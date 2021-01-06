#include <VEngine/Headers/physics.h>
#include <stdlib.h>

Ball Ball_set(Vec2 position)
{
    Ball ball = { 0 };

    ball.position = position;
    ball.velocity = Vec2_set(0.0f, 0.0f);
    ball.mass = 0.5f;
    ball.friction = 0.75f;

    return ball;
}

int Ball_connect(Ball *ball1, Ball *ball2, float length)
{
    if ((ball1 == ball2)
        || (ball1->nbSprings >= MAX_EDGES)
        || (ball2->nbSprings >= MAX_EDGES))
        return EXIT_FAILURE;

    ball1->springs[ball1->nbSprings].other = ball2;
    ball2->springs[ball2->nbSprings].other = ball1;
    ball1->springs[ball1->nbSprings].length = length;
    ball2->springs[ball2->nbSprings].length = length;

    ball1->nbSprings++;
    ball2->nbSprings++;

    return EXIT_SUCCESS;
}

int Ball_deconnect(Ball *ball1, Ball *ball2)
{
    int nbSprings1 = ball1->nbSprings;
    int nbSprings2 = ball2->nbSprings;
    int exitStatus = EXIT_SUCCESS;
    int i;

    // Remove ball2 from the egdes of ball1
    for (i = 0; i < nbSprings1; ++i)
    {
        if (ball1->springs[i].other == ball2)
        {
            ball1->springs[i] = ball1->springs[nbSprings1 - 1];
            ball1->nbSprings--;
            break;
        }
    }
    if (i >= nbSprings1)
    {
        exitStatus = EXIT_FAILURE;
    }

    // Remove ball1 from the egdes of ball2
    for (i = 0; i < nbSprings2; ++i)
    {
        if (ball2->springs[i].other == ball1)
        {
            ball2->springs[i] = ball2->springs[nbSprings2 - 1];
            ball2->nbSprings--;
            break;
        }
    }
    if (i >= nbSprings2)
    {
        exitStatus = EXIT_FAILURE;
    }

    return exitStatus;
}

void Ball_updateVelocity(Ball *ball, float timeStep)
{

    Vec2 totalSpringForces = { .x = 0.0f, .y = 0.0f};

    // Calculate each spring force and sum them up
    for(int i = 0; i < ball->nbSprings; ++i){

        Vec2 center = ball->springs[i].other->position;

        float k = 300.0f;
        Vec2 I = Vec2_normalize(Vec2_sub(center, ball->position));
        float l = Vec2_distance(center, ball->position);
        float l0 = ball->springs[i].length;

        totalSpringForces = Vec2_add(totalSpringForces, Vec2_scale(I, (l - l0) * k));

    }

    // P = m * g
    Vec2 weight = { .x = 0, .y = ball->mass * -9.81f };
    // f = -alpha*v
    Vec2 friction = Vec2_scale(ball->velocity, -ball->friction);
    // a = sum(forces) / mass
    Vec2 acceleration = Vec2_scale(Vec2_add3(friction, totalSpringForces, weight),1 / ball->mass);

    // v(t) = v(0) + a(t) * t
    ball->velocity = Vec2_add(ball->velocity, Vec2_scale(acceleration, timeStep));

}

void Ball_updatePosition(Ball *ball, float timeStep)
{
    // X(t) = X(0) + v(t) * t
    ball->position = Vec2_add(ball->position, Vec2_scale(ball->velocity, timeStep));
    if (ball->position.y < 0 )
    {
        // Bounce the ball when touching the ground
        ball->velocity.y *= -1;
        ball->position.y=0;
    }
}
