-------------------------------------------------------------------------------------------------

ALWAYS

open terminal

cd .. /Blackjack

-------------------------------------------------------------------------------------------------

BEFORE START

chmod 777 compilefifos.sh
chmod 777 compilesockets.sh

./compilefifos.sh  // ./compilesockets.sh

-------------------------------------------------------------------------------------------------

START SERVER
./server

-------------------------------------------------------------------------------------------------

START LOGGING SERVER (OPTIONAL)
./loggingServer

-------------------------------------------------------------------------------------------------

START CLIENT
./client

-------------------------------------------------------------------------------------------------

--> You can open as many clients as you like. They will connect every time a new round starts if there are available seats.

--> To configure game rules, open config.h
