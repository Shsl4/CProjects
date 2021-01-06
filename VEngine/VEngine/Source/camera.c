#include <VEngine/Headers/camera.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Camera_updateScales(Camera *camera);

Rect Rect_set(float x, float y, float w, float h)
{
    Rect rect = { .x = x, .y = y, .w = w, .h = h };
    return rect;
}

Camera *Camera_new(int width, int height)
{
    Camera *camera = NULL;

    camera = (Camera *)calloc(1, sizeof(Camera));
    if (!camera) goto ERROR_LABEL;

    camera->m_width = width;
    camera->m_height = height;
    camera->m_xScale = 1.0f;
    camera->m_yScale = 1.0f;
    camera->m_worldView = Rect_set(0.f, 0.f, 16.f, 9.f);

    Camera_updateScales(camera);

    return camera;

ERROR_LABEL:
    printf("ERROR - Camera_new()\n");
    return NULL;
}

void Camera_free(Camera *camera)
{
    if (!camera) return;

    memset(camera, 0, sizeof(Camera));
    free(camera);
}

void Camera_updateScales(Camera *camera)
{
    Rect *worldView = &camera->m_worldView;
    float viewWidth, viewHeight;
    viewWidth  = worldView->w;
    viewHeight = worldView->h;

    camera->m_xScale = camera->m_width  / viewWidth;
    camera->m_yScale = camera->m_height / viewHeight;
}

void Camera_zoom(Camera *camera, Vec2 center, float factor)
{
    Rect *worldView = &camera->m_worldView;
    worldView->x = center.x + (worldView->x - center.x) * factor;
    worldView->y = center.y + (worldView->y - center.y) * factor;
    worldView->w *= factor;
    worldView->h *= factor;
    Camera_updateScales(camera);
}

void Camera_worldToView(Camera *camera, Vec2 position, int *x, int *y)
{
    *x = (int)((position.x - camera->m_worldView.x) * camera->m_xScale);
    *y = (int)((position.y - camera->m_worldView.y) * camera->m_yScale);
    *y = camera->m_height - *y;
}

void Camera_viewToWorld(Camera *camera, int x, int y, Vec2 *position)
{
    y = camera->m_height - y;
    float ratioX = (float)x / (float)camera->m_width;
    float ratioY = (float)y / (float)camera->m_height;
    position->x = camera->m_worldView.x + ratioX * (camera->m_worldView.w);
    position->y = camera->m_worldView.y + ratioY * (camera->m_worldView.h);
}

void Camera_move(Camera *camera, Vec2 displacement)
{
    camera->m_worldView.x += displacement.x;
    camera->m_worldView.y += displacement.y;
}

void Camera_setView(Camera *camera, Rect *worldView)
{
#ifdef _WIN32
    memcpy_s(&camera->m_worldView, sizeof(Rect), worldView, sizeof(Rect));
#else
    memcpy(&camera->m_worldView, worldView, sizeof(Rect));
#endif
    Camera_updateScales(camera);
}

void Camera_getView(Camera *camera, Rect *worldView)
{
#ifdef _WIN32
    memcpy_s(worldView, sizeof(Rect), &camera->m_worldView, sizeof(Rect));
#else
    memcpy(worldView, &camera->m_worldView, sizeof(Rect));
#endif
}
