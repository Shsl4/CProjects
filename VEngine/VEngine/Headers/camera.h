
#ifndef _EX_CAMERA_H_
#define _EX_CAMERA_H_

/// @file camera.h
/// @defgroup Camera
/// @{

#include "vector.h"

/// @brief Structure représenant un rectangle 2D alligné sur les axes X et Y.
typedef struct Rect_s
{
    /// @brief Abscisse du sommet en bas à gauche du rectangle.
    float x;

    /// @brief Ordonnée du sommet en bas à gauche du rectangle.
    float y;

    /// @brief Largeur du rectangle.
    float w;

    /// @brief Hauteur du rectangle.
    float h;
} Rect;

/// @brief Construit un rectangle.
/// @param[in] x abscisse du sommet en bas à gauche du rectangle.
/// @param[in] y ordonnée du sommet en bas à gauche du rectangle.
/// @param[in] w largeur du rectangle.
/// @param[in] h hauteur du rectangle.
/// @return Le rectangle spécifié.
Rect Rect_set(float x, float y, float w, float h);

/// @brief Structure représentant la caméra par laquelle est vue la scène.
typedef struct Camera_s
{
    Rect m_worldView;
    int m_width;
    int m_height;
    float m_xScale;
    float m_yScale;
} Camera;

/// @brief Crée une caméra.
/// @param[in] width la largeur en pixels de la caméra.
/// @param[in] height la hauteur en pixels de la caméra.
/// @return La caméra ou NULL en cas d'erreur.
Camera *Camera_new(int width, int height);

/// @brief Détruit une caméra préalablement allouée avec Camera_new().
/// @param[in,out] camera la caméra à détruire.
void Camera_free(Camera *camera);

/// @brief Définit la zone du monde vue par la caméra.
/// @param[in,out] camera la caméra.
/// @param[in] worldView le rectangle exprimé dans le référentiel monde.
void Camera_setView(Camera *camera, Rect *worldView);

/// @brief Renvoie la zone du monde vue par la caméra.
/// @param[in] camera la caméra.
/// @param[out] worldView le rectangle vu par la caméra exprimé dans le référentiel monde.
void Camera_getView(Camera *camera, Rect *worldView);

/// @brief Réduit ou augmente la zone du monde vue par la caméra.
/// @param[in,out] camera la caméra.
/// @param[in] center le centre de l'homothétie. Il s'agit du point non modifié par la transformation.
/// @param[in] factor le facteur de zoom.
void Camera_zoom(Camera *camera, Vec2 center, float factor);

/// @brief Transforme des coordonnées exprimées dans le référentiel monde vers le référentiel caméra.
/// @param[in] camera la caméra.
/// @param[in] position la position d'un point dans le référentiel monde.
/// @param[out] x l'abscisse du point dans le référentiel caméra (en pixels).
/// @param[out] y l'ordonnée du point dans le référentiel caméra (en pixels).
void Camera_worldToView(Camera *camera, Vec2 position, int *x, int *y);

/// @brief Transforme des coordonnées exprimées dans le référentiel caméra vers le référentiel monde.
/// @param[in] camera camera la caméra.
/// @param[in] x l'abscisse d'un point dans le référentiel caméra (en pixels).
/// @param[in] y l'ordonnée d'un point dans le référentiel caméra (en pixels).
/// @param[out] position la position du point dans le référentiel monde.
void Camera_viewToWorld(Camera *camera, int x, int y, Vec2 *position);

/// @brief Déplace la caméra.
/// @param[in,out] camera la caméra.
/// @param[out] displacement le vecteur de déplacement exprimé dans le référentiel monde.
void Camera_move(Camera *camera, Vec2 displacement);

/// @}

#endif
