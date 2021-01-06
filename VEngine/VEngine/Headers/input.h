//
// Created by sl4sh on 1/5/21.
//

#ifndef VENGINE_VECTOR
#define VENGINE_VECTOR

#include "vector.h"

/// @brief Initialise le système d'input
void initInput();

/// @brief Fonction appelée à chaque frame
void tickInput();

/// @brief Retourne la valeur de mouseX
/// @param[out] mouseX la valeur sur l'abcisse de la position de la souris
int getMouseX();

/// @brief Retourne la valeur de mouseY
/// @param[out] mouseY la valeur sur l'ordonnée de la position de la souris
int getMouseY();

/// @brief Retourne la valeur de mouseDelta
/// @param[out] mouseDelta la différence entre chaque frame rate de la position de la souris
Vec2 getMouseDelta();

/// @brief Initialise la valeur de mouseDelta
/// @param[in] mouseDelta la différence entre chaque frame rate de la position de la souris
void setMouseDelta(Vec2 inputMouseDelta);

/// @}
#endif
