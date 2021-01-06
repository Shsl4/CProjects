#ifndef _PHYSICS_H_
#define _PHYSICS_H_

/// @file physics.h
/// @defgroup Physics
/// @{

#include "vector.h"

#define MAX_EDGES 10

typedef struct Ball_s Ball;

/// @brief Structure représentant un ressort entre une balle et son point d'attache.
typedef struct Spring_s
{
    /// @brief Pointeur vers le point d'attache.
    Ball *other;

    /// @brief Longueur du ressort au repos.
    float length;
} Spring;

/// @brief Structure représentant une balle. Elle peut être liée à d'autres balle avec des ressorts.
struct Ball_s
{
    /// @brief Vecteur position de la balle.
    Vec2 position;

    /// @brief Vecteur vitesse de la balle.
    Vec2 velocity;

    /// @brief Masse de la balle (exprimée en kg).
    float mass;

    /// @brief Coefficient de friction de la balle.
    float friction;

    /// @brief Nombre de ressorts liant la balle à d'autres balles.
    int nbSprings;

    /// @brief Tableau contenant les ressorts attachés à la balle.
    Spring springs[MAX_EDGES];

};

/// @brief Initialise une balle et permet de définir sa position.
/// @param[in] position le vecteur position de la balle.
/// @return La balle initialisée.
Ball Ball_set(Vec2 position);

/// @brief Lie deux balles avec un ressort dont la longueur au repos est spécifiée.
/// @param[in,out] ball1 la première balle.
/// @param[in,out] ball2 la seconde balle.
/// @param[in] length la longueur au repos du ressort.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int Ball_connect(Ball *ball1, Ball *ball2, float length);

/// @brief Supprime le ressort liant deux balles.
/// @param[in,out] ball1 la première balle.
/// @param[in,out] ball2 la seconde balle.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int Ball_deconnect(Ball *ball1, Ball *ball2);

/// @brief Met à jour la vitesse d'une balle en fonction des forces qui lui sont appliquées.
/// @param[in,out] ball la balle à mettre à jour.
/// @param[in] timeStep le pas de temps.
void Ball_updateVelocity(Ball *ball, float timeStep);

/// @brief Met à jour la position d'une balle en fonction de sa vitesse.
/// @param[in,out] ball la balle à mettre à jour.
/// @param[in] timeStep le pas de temps.
void Ball_updatePosition(Ball *ball, float timeStep);

/// @}

#endif