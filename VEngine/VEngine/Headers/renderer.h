
#ifndef _RENDERER_H_
#define _RENDERER_H_

/// @file renderer.h
/// @defgroup Renderer
/// @{

#include <VEngine/Headers/VEngine.h>
#include <VEngine/Headers/vector.h>

#define TEXT_DRAW_HEIGHT 22

extern SDL_Color White;

/// @brief Structure représentant la moteur de rendu.
typedef struct Renderer_s
{
    /// @brief Le moteur de rendu SDL.
    SDL_Renderer *m_rendererSDL;

    /// @brief La largeur en pixels de l'image rendue.
    int m_width;

    /// @brief La hauteur en pixels de l'image rendue.
    int m_height;
} Renderer;

/// @brief Structure représentant une couleur au format RGBA.
typedef struct Color_s
{
    /// @brief L'intensité de la composante rouge entre 0 et 255.
    uint8_t r;

    /// @brief L'intensité de la composante verte entre 0 et 255.
    uint8_t g;

    /// @brief L'intensité de la composante bleue entre 0 et 255.
    uint8_t b;

    /// @brief La composante alpha (transparence) de la couleur.
    /// La couleur est parfaitement transparante quand elle vaut 0 et elle est opaque quand elle vaut 255.
    uint8_t a;
} Color;

/// @brief Construit une couleur à partir de ses composantes [R,V,B,A] chacune entre 0 et 255.
/// @param[in] r l'intensité du rouge.
/// @param[in] g l'intensité du vert.
/// @param[in] b l'intensité du bleu.
/// @param[in] a la composante alpha (transparence) de la couleur.
/// La couleur est parfaitement transparante quand elle vaut 0 et elle est opaque quand elle vaut 255.
/// @return La couleur spécifiée.
Color makeColor(int r, int g, int b, int a);

Color randomColor();

/// @brief Met à jour le moteur de rendu.
/// Cette fonction permet de d'afficher à l'écran la dernière image rendue.
/// @param[in,out] renderer le moteur de rendu.
void Renderer_update(Renderer* renderer);

/// @brief Efface le rendu précédent. Le rendu devient entièrement noir.
/// @param[in,out] renderer le moteur de rendu.
void Renderer_clear(Renderer* renderer);

/// @brief Remplit le rendu avec une couleur donnée.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] color la couleur de remplissage.
void Renderer_fill(Renderer* renderer, Color color);

/// @brief Dessine un segment spécifié par deux points sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] x1 abscisse du premier point (en pixels).
/// @param[in] y1 ordonnée du premier point (en pixels).
/// @param[in] x2 abscisse du second point (en pixels).
/// @param[in] y2 ordonnée du second point (en pixels).
/// @param[in] color la couleur de la ligne.
void Renderer_drawLine(Renderer *renderer, int x1, int y1, int x2, int y2, Color color);

/// @brief Dessine un point sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] x abscisse du point (en pixels).
/// @param[in] y ordonnée du point (en pixels).
/// @param[in] color la couleur du point.
void Renderer_drawPoint(Renderer *renderer, int x, int y, Color color);

/// @brief Renvoie la largeur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @return La largeur du moteur de rendu.
int Renderer_getWidth(Renderer *renderer);

/// @brief Renvoie la hauteur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @return La hauteur du moteur de rendu.
int Renderer_getHeight(Renderer *renderer);

/// @brief Affiche du texte à l'écran a partir du coin gauche de l'écran
/// @param[in,out] renderer le moteur de rendu
/// @param[in] pos la position d'affichage du texte
/// @param[in] font la police d'écriture du texte
/// @param[in] *text la chaîne de caractère à afficher
/// @param[in] color la couleur d'affichage du texte
/// @param[in] freeStr libere ou non la chaine 'text' automatiquement
void drawText(SDL_Renderer* renderer, Vec2 pos, TTF_Font* font, char *text, SDL_Color color, bool freeText);

/// @brief Affiche du texte à l'écran a partir du coin droit de l'écran
/// @param[in,out] renderer le moteur de rendu
/// @param[in] pos la position d'affichage du texte
/// @param[in] font la police d'écriture du texte
/// @param[in] *text la chaîne de caractère à afficher
/// @param[in] color la couleur d'affichage du texte
/// @param[in] freeStr libere ou non la chaine 'text' automatiquement
void drawText_FromEnd(SDL_Renderer* renderer, Vec2 pos, TTF_Font* font, char *text, SDL_Color color, bool freeText);


/// @}
#endif
