/**
 * \file MyTicker.h
 * \page tickers Tickers
 * \brief Repassez dans 5 minutes s'il vous plaît
 * 
 * Un Ticker est un objet qui est appelé à une fréquence définie.
 * Chaque Ticker est associé à une fonction qu'il va appeler quand il est l'heure.
 * Important : un ticker n'interrompt pas l'exécution du code, il se "glisse" au milieu
 * et est exécuté dès que le processeur a le temps de le faire !
 * 
 * Vous pouvez définir autant de Ticker que vous le souhaitez, dans la limite de la mémoire.
 * 
 * Vous pouvez attacher et détacher une fonction à un Ticker, ou changer de fonction ou changer
 * la fréquence à souhait, par exemple quand un paramètre est modifié.
 * 
 * Il y a 2 façons d'attacher une fonction avec attach(secondes) et attach_ms(milliseconds)
 * 
 * A noter que comme on ne connaît pas l'heure (et que ce n'est pas forcément nécessaire dans
 * ce cas), o
 * 
 * Fichier \ref MyTicker.h
 */
 
#include <Ticker.h>

Ticker myTicker;

void setupTicker(){
}