#include <VEngine/Headers/timer.h>
#include <VEngine/Headers/VEngine.h>

Timer* Timer_new()
{
    Timer* timer = NULL;

    timer = (Timer*)calloc(1, sizeof(Timer));
    if (!timer)
        return NULL;

    timer->m_startTime = -1.f;
    timer->m_currentTime = 0.f;
    timer->m_previousTime = timer->m_currentTime;
    timer->m_delta = 0.f;

    return timer;
}

void Timer_free(Timer* timer)
{
    if (!timer) return;

    memset(timer, 0, sizeof(Timer));
    free(timer);
}

void Timer_start(Timer* timer)
{
    if (!timer)
        return;

    timer->m_startTime = SDL_GetTicks() / 1000.f;
    timer->m_currentTime = 0.f;
    timer->m_previousTime = 0.f;
    timer->m_delta = 0.f;
}

void Timer_update(Timer* timer)
{
    timer->m_previousTime = timer->m_currentTime;
    timer->m_currentTime = SDL_GetTicks() / 1000.f - timer->m_startTime;
    timer->m_delta = timer->m_currentTime - timer->m_previousTime;
}

float Timer_getDelta(Timer *timer)
{
    return timer->m_delta;
}

float Timer_getElapsed(Timer *timer)
{
    return timer->m_currentTime - timer->m_startTime;
}