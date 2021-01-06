
#ifndef _TIMER_H_
#define _TIMER_H_

/// @file timer.h
/// @defgroup Timer
/// @{

/// @brief Structure représentant un chronomètre.
typedef struct Timer_s
{
    /// @brief Temps de départ.
    float m_startTime;

    /// @brief Temps du dernier appel à Timer_update().
    float m_currentTime;

    /// @brief Temps de l'avant dernier appel à Timer_update().
    float m_previousTime;

    /// @brief Ecart entre les deux derniers appels à Timer_update().
    float m_delta;
} Timer;

/// @brief Crée un nouveau timer.
/// @return Le timer créé.
Timer* Timer_new();

/// @brief Détruit un timer préalablement alloué avec Timer_new().
/// @param[in,out] timer le timer à détruire.
void Timer_free(Timer* timer);

/// @brief Lance le timer.
/// @param[in,out] timer le timer.
void Timer_start(Timer* timer);

/// @brief Met à jour le timer en fonction du temps actuel.
/// @param[in,out] timer le timer.
void Timer_update(Timer* timer);

/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers appels à la fonction
/// Timer_update().
/// @param[in] timer le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
float Timer_getDelta(Timer *timer);

/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer avec la fonction
/// Timer_start() et le dernier appel à la fonction Timer_update().
/// @param[in] timer le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la dernière mise à jour.
float Timer_getElapsed(Timer *timer);

#endif