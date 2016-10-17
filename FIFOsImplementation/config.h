#ifndef CONFIG_H
#define CONFIG_H

/* Server Configuration file */

/* Maximum amount of players allowed on BlackJack Table (Int > 1) */
#define PLAYERS 4

/* Starting Balance for new players (Int > 1) */
#define STARTING_BALANCE 100

/* Amount of decks (BlackJack uses many 52-Cards decks to play) (Int > 1) */
#define PLAYING_DECKS 6

/* How far the deck will go until it is shuffled (Between 0.1 and 0.8)
(E. 0.7 means when deck is 70% used, it shuffles) */
#define DECK_PENETRATION 0.7

#endif