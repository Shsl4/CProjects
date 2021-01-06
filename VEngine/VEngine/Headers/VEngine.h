//
// Created by sl4sh on 1/5/21.
//

#ifndef VENGINE
#define VENGINE

// Enables asprintf
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TCOLOR_RED     "\x1b[31m"
#define TCOLOR_GREEN   "\x1b[32m"
#define TCOLOR_YELLOW  "\x1b[33m"
#define TCOLOR_RESET   "\x1b[0m"

#define LOG_INFO 0
#define LOG_SUCCESS 1
#define LOG_WARNING 2
#define LOG_ERROR 3
#define LOG_FATAL 4

#define VE_LOG(format, severity, ...) VEngine_Log(format, severity, ##__VA_ARGS__)

#define getWindow() getEngine().window
#define getRenderer() getEngine().window->m_renderer
#define getSDLRenderer() getEngine().window->m_renderer->m_rendererSDL

#define bool int
#define true 1
#define false 0

///@brief Structure représentant les paramètres globales du jeu
typedef struct Game_s{
    ///@brief Variable contenant le nom du jeu
    char* gameName;
    ///@brief Pointeur vers une fonction appelée lors du lancement du moteur
    void* playFunction;
    ///@brief Pointeur vers une fonction appelée a chaque rendu d'image
    void* tickFunction;
    ///@brief Pointeur vers une fonction appelée lorsque le moteur s'arrete
    void* onQuitFunction;
    ///@brief Pointeur vers une fonction appelée dans tickInput (void input.h)
    void* inputProcessFunction;

} Game;

///@brief Pré-déclaration de la fenêtre (pour éviter une dépendance circulaire)
typedef struct Window_s Window;

///@brief Structure contenant les informations globales du moteur
typedef struct VEngine_s{
    ///@brief Pointeur vers une variable Game représentant le jeu
    Game* game;
    ///@brief Pointeur vers la fenêtre de jeu
    Window* window;

} VEngineData;

///@brief énumération d'ID pour les rapports
typedef enum LogSeverity_e {

    Info = 0,
    Success = 1,
    Warning = 2,
    Error = 3,
    Fatal = 4

} LogSeverity;

///@brief defaultFont Variable globale represéntant la police d'écriture par défaut
extern TTF_Font* defaultFont;

///@brief Recherche le moteur du jeu
///@param[out] engineData les données du moteur
VEngineData getEngine(void);

///@brief Retourne le temps passé à chaque frame
///@param[out] deltaTime différence de temps entre deux frames
float getDeltaTime();

///@brief Fonction qui arrête le moteur
void VEngine_Stop();

///@brief Imprime une ligne dans la console
///@param[in] format format d'impression de la ligne
///@param[in] severity  Type d'information imprimé
///@param[in] ... arguments variables
void VEngine_Log(char* format, LogSeverity severity, ...);

///@brief Crée une instance du moteur
void VEngine(Game game);

/// @}
#endif