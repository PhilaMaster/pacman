# Progetto Pacman
Questo progetto è un'implementazione del gioco Pac-Man, sviluppato come parte del corso di Architetture dei Sistemi di Elaborazione. Il gioco è stato sviluppato per l'emulatore LandTiger, con un focus sull'implementazione della logica di gioco e sulla gestione delle risorse hardware limitate del dispositivo.

# Requisiti
Keil µVision: Per la programmazione e il debugging del progetto.
LandTiger Board: (Se disponibile fisicamente) per testare il progetto sul dispositivo reale.

# Specifiche del Gioco
Labirinto: Il labirinto è composto da 240 pillole standard e 6 pillole Power Pills generate in posizioni casuali. Le pillole Power Pills sostituiscono quelle standard.
Movimento di Pac-Man: Il giocatore può spostare Pac-Man tramite il joystick, scegliendo la direzione. Il movimento continua fino a quando non si incontra un muro o il giocatore sceglie una nuova direzione.
Punteggio e Vite: Ogni volta che Pac-Man mangia una pillola standard, guadagna 10 punti, mentre ogni Power Pill gli dà 50 punti. Ogni 1000 punti guadagnati, il giocatore ottiene una nuova vita.
Teleportazione: Quando Pac-Man raggiunge la zona di teletrasporto, può attraversare il labirinto da un lato all'altro mantenendo la stessa direzione.
Pause e Countdown: Il gioco inizia in modalità "PAUSE". È possibile mettere in pausa e riprendere il gioco tramite un interruttore. Inoltre, c'è un timer che parte da 60 secondi.
Condizioni di Fine Gioco: Se Pac-Man mangia tutte le pillole, viene visualizzata una schermata di "Vittoria". Se il timer scade prima che tutte le pillole siano mangiate, viene mostrato un messaggio di "Game Over".
