
#ifndef _VECTOR_H_
#define _VECTOR_H_

/// @file vector.h
/// @defgroup Vector
/// @{

/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
typedef struct Vec2_s
{
    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;
} Vec2;

/// @brief Construit un vecteur à partir de ses composantes.
/// @param[in] x la composante x.
/// @param[in] y la composante y.
/// @return Le vecteur ayant les composantes données.
Vec2 Vec2_set(float x, float y);

/// @brief Aditionne deux vecteurs.
/// @param[in] v1 le premier vecteur.
/// @param[in] v2 le second vecteur.
/// @return La somme de v1 et v2.
Vec2 Vec2_add(Vec2 v1, Vec2 v2);

Vec2 Vec2_Negate(Vec2 v);

Vec2 Vec2_add3(Vec2 v1, Vec2 v2, Vec2 v3);

/// @brief Soustrait deux vecteurs.
/// @param[in] v1 le premier vecteur.
/// @param[in] v2 le second vecteur.
/// @return La différence de v1 par v2.
Vec2 Vec2_sub(Vec2 v1, Vec2 v2);

/// @brief Multiplie un vecteur par un scalaire.
/// @param[in] v le vecteur.
/// @param[in] s le scalaire.
/// @return Le produit de s et v.
Vec2 Vec2_scale(Vec2 v, float s);

/// @brief Calcule la norme d'un vecteur.
/// @param[in] v le vecteur.
/// @return La norme euclidienne de v.
float Vec2_norm(Vec2 v);

/// @brief Normalise un vecteur.
/// @param[in] v le vecteur.
/// @return Le vecteur unitaire de même direction.
Vec2 Vec2_normalize(Vec2 v);

/// @brief Renvoie la distance entre deux points.
/// @param[in] v1 les coordonnées du premier point.
/// @param[in] v2 les coordonnées du second point.
/// @return La distance séparant les deux points.
float Vec2_distance(Vec2 v1, Vec2 v2);

#endif