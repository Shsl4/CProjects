
#ifndef _SCENE_H_
#define _SCENE_H_

/// @file scene.h
/// @defgroup Scene
/// @{

typedef struct Camera_s Camera;
typedef struct Renderer_s Renderer;
typedef struct Ball_s Ball;
typedef struct Vec2_s Vec2;

/// @brief Structure représentant la scène de la simulation.
/// Elle contient la caméra par laquelle la scène est vue ainsi que toutes les balles.
typedef struct Scene_s
{
    /// @brief La caméra par laquelle la scène est vue.
    Camera *m_camera;

    /// @brief Le moteur de rendu.
    Renderer *m_renderer;

    /// @brief Tableau contenant les balles présentes dans la scène.
    Ball *m_balls;

    /// @brief Le nombre de balles dans la scène.
    int m_nbBalls;

    /// @brief Le nombre maximal de balles avant d'effectuer une réallocation mémoire.
    int m_capacity;
} Scene;

/// @brief Construit une scène.
/// @param[in] renderer le moteur de rendu.
/// @return La scène créée. Renvoie NULL en cas d'erreur.
Scene *Scene_new(Renderer *renderer);

/// @brief Détruit une scène précédemment construite avec Scene_new().
/// Le pointeur vers la scène doit être affecté à NULL après l'appel à cette fonction.
/// @param[in,out] scene la scène à détruire.
void Scene_free(Scene *scene);

/// @brief Renvoie le moteur de rendu associé à la scène.
/// @param[in] scene la scène.
/// @return Le moteur de rendu associé à la scène.
Renderer *Scene_getRenderer(Scene *scene);

/// @brief Renvoie la caméra par laquelle la scène est vue.
/// @param[in] scene la scène.
/// @return La caméra associée à la scène.
Camera *Scene_getCamera(Scene *scene);

/// @brief Ajoute une balle à la scène.
/// @param[in,out] scene la scène.
/// @param[in] position la position de la nouvelle balle.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
Ball *Scene_addBall(Scene *scene, Vec2 position);

/// @brief Supprime une balle de la scène.
/// Les adresses des balles sont invalidées après l'appel à cette fonction.
/// @param[in,out] scene la scène.
/// @param[in,out] ball pointeur vers la balle à supprimer.
void Scene_removeBall(Scene *scene, Ball *ball);

/// @brief Renvoie le nombre de balles présentes dans la scène.
/// @param[in] scene la scène.
/// @return Le nombre de balles présentes dans la scène.
int Scene_getNbBalls(Scene *scene);

/// @brief Renvoie le tableau contenant les balles présentes dans la scène.
/// La taille de ce tableau s'obtient avec la fonction Scene_getNbBalls().
/// @param[in] scene la scène.
/// @return Le tableau contenant les balles présentes dans la scène.
Ball *Scene_getBalls(Scene *scene);

/// @brief Met à jour les positions des balles présentes dans la scène.
/// Cette fonction correspond au moteur physique.
/// @param[in,out] scene la scène.
/// @param[in] timeStep le pas de temps.
void Scene_updateBalls(Scene *scene, float timeStep);

/// @brief Calcule le rendu de la scène vue par sa caméra.
/// @param[in] scene la scène à rendre.
void Scene_renderBalls(Scene *scene);

//-------------------------------------------------------------------------------------------------
// Fonctions de recherche

/// @brief Structure représentant le résultat d'une recherche de balle.
typedef struct BallQuery_s
{
    /// @brief Pointeur vers la balle.
    Ball *ball;

    /// @brief Distance entre la balle et la position donnée.
    float distance;

} BallQuery;

/// @brief Recherche dans une scène la balle la plus proche d'une position donnée.
/// @param[in] scene la scène dans laquelle faire la recherche.
/// @param[in] position la position autour de laquelle faire la recherche.
/// @return Une structure BallQuery contenant un pointeur vers la balle la plus proche
/// ainsi que sa distance avec la position spécifiée.
/// Si la scène ne contient aucune balle, le pointeur vers la balle vaut NULL.
BallQuery Scene_getNearestBall(Scene *scene, Vec2 position);

/// @brief Recherche dans une scène les balles les plus proches d'une position donnée.
/// @param[in] scene la scène dans laquelle faire la recherche.
/// @param[in] position la position autour de laquelle faire la recherche.
/// @param[out] queries tableau dans lequel vont être écrit les résultats.
/// Il doit contenir au moins nbQueries cases.
/// @param[in] nbQueries le nombre de balles à rechercher.
/// @return EXIT_SUCCESS ou EXIT_FAILURE. Le resultat de la recherche est écrit dans le tableau queries.
int Scene_getNearestBalls(Scene *scene, Vec2 position, BallQuery *queries, int nbQueries);

/// @brief Recherche dans une scène les balles les plus proches d'une position donnée.
/// @param[in] scene la scène dans laquelle faire la recherche.
/// @param[in] position la position autour de laquelle faire la recherche.
/// @param[in] distanceThrehold distance max de détection de balle
/// @param[out] queries tableau dans lequel vont être écrit les résultats.
/// @param[in] minQueries le nombre minimal de balles a trouver
/// @param[in] maxQueries le nombre maximal de balles a trouver
/// @param[out] numBallsFound le nombre de balles attachables trouvées à proximité de la souris
/// @return EXIT_SUCCESS ou EXIT_FAILURE. Le resultat de la recherche est écrit dans le tableau queries et numBallsFound.
int Scene_getNearestBallDynamic(Scene* scene, Vec2 position, float distanceThreshold, BallQuery** queries, int minQueries, int maxQueries, int* numBallsFound);

/// @}
#endif
