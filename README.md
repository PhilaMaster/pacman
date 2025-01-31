# Pac-Man Project
This project is an implementation of the Pac-Man game, developed as part of the "Architetture dei Sistemi Di Elaborazione" course. The game was developed for the LandTiger board, focusing on implementing game logic and managing the device's limited hardware resources.

**Game screen running on the simulator:**
![image](https://github.com/user-attachments/assets/a9315584-7677-494f-ac67-b92c433764b9)

## Requirements
- **Keil µVision:** Used for programming and debugging the project.
- **LandTiger Board:** Used to test the project on the real device, if physically available.
- To ensure proper random map generation, it is recommended to set the potentiometer to any position except the maximum. Otherwise, low noise may cause the same pills generation multiple times.

## Game Specifications
- **Maze:** The maze consists of 240 standard pills and 6 Power Pills generated in random positions. The Power Pills replace the standard ones.
- **Pac-Man Movement:** The player can move Pac-Man using the joystick to choose a direction. The movement continues until hitting a wall or selecting a new direction.
- **Score and Lives:** Each standard pill grants 10 points, while each Power Pill gives 50 points. Every 1000 points earned grants the player an extra life.
- **Teleportation:** When Pac-Man reaches the teleportation zone, he can cross the maze from one side to the other while maintaining the same direction.
- **Pause and Countdown:** The game starts in "PAUSE" mode. It can be paused and resumed using the INT0 button. Additionally, a countdown timer starts from 60 seconds.
- **Game Over Conditions:** If Pac-Man eats all the pills, a "Victory" screen is displayed. If the timer runs out before all pills are eaten, a "Game Over" message is shown.

## Additional Features
- **AI-Controlled Ghost:**
  - A ghost named Blinky (Red) pursues Pac-Man, causing him to lose a life on contact.
  - Blinky's AI features two behavior modes:
    1. **Chase Mode:** The ghost actively tries to catch Pac-Man.
    2. **Frightened Mode:** Activated when Pac-Man eats a Power Pill. The ghost turns blue for 10 seconds, runs away, and can be eaten for an extra 100 points. If eaten, it respawns in its central spawning point after 3 seconds.
  - The ghost's AI is implemented using a Breadth-first Search to determine the shortest path to its target.

- **Sound Effects and Music:**
  - The speaker is configured to play sound effects and background music during gameplay.

- **CAN Bus Integration:**
  - The CAN peripheral is used to display the current score, remaining lives, and countdown timer at the end of the game, after clicking KEY1.
  - The LandTiger Board's CAN controllers is set to external "loopback" mode. This means that CAN1 sends messages while CAN2 receives them, simulating communication with another board.
  - The message is encoded as follows:
    - **Remaining Time:** 8 bits
    - **Remaining Lives:** 8 bits
    - **Score:** 16 bits
  - Every message is stored in a 32-bit unsigned integer variable.


