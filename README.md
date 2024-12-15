# Progetto Pacman
Questo progetto è un'implementazione del gioco Pac-Man, sviluppato come parte del corso di Architetture dei Sistemi di Elaborazione. Il gioco è stato sviluppato per la scheda LandTiger, con un focus sull'implementazione della logica di gioco e sulla gestione delle risorse hardware limitate del dispositivo.

<b>Schermata del gioco eseguito sul simulatore:</b>
![image](https://github.com/user-attachments/assets/a9315584-7677-494f-ac67-b92c433764b9)


# Requisiti
Keil µVision: Per la programmazione e il debugging del progetto.
LandTiger Board: (Se disponibile fisicamente) per testare il progetto sul dispositivo reale.


# Specifiche del Gioco
<ul>
  <li><b>Labirinto:</b> Il labirinto è composto da 240 pillole standard e 6 pillole Power Pills generate in posizioni casuali. Le pillole Power Pills sostituiscono quelle standard.</li>
  <li><b>Movimento di Pac-Man:</b> Il giocatore può spostare Pac-Man tramite il joystick, scegliendo la direzione. Il movimento continua fino a quando non si incontra un muro o il giocatore sceglie una nuova direzione.</li>
  <li><b>Punteggio e Vite:</b> Ogni volta che Pac-Man mangia una pillola standard, guadagna 10 punti, mentre ogni Power Pill gli dà 50 punti. Ogni 1000 punti guadagnati, il giocatore ottiene una nuova vita.</li>
  <li><b>Teletrasporto:</b> Quando Pac-Man raggiunge la zona di teletrasporto, può attraversare il labirinto da un lato all'altro mantenendo la stessa direzione.</li>
  <li><b>Pause e Countdown:</b> Il gioco inizia in modalità "PAUSE". È possibile mettere in pausa e riprendere il gioco tramite il pulsante INT0. Inoltre, c'è un timer che parte da 60 secondi.</li>
  <li><b>Condizioni di Fine Gioco:</b> Se Pac-Man mangia tutte le pillole, viene visualizzata una schermata di "Vittoria". Se il timer scade prima che tutte le pillole siano mangiate, viene mostrato un     messaggio di "Game Over".</li>

</ul>
