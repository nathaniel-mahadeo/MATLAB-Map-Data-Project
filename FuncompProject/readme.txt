Name: Nathaniel Mahadeo
Assignment: Lab 11 – Mini Final Project (readme.txt)

This project is a small interactive game built using gfx graphics
and the C programming language. It expands on concepts from earlier
labs by integrating structs, animation, collision detection, state
machines, and event-driven input.

This is modeled off of Jujutsu Kaisen, specifically the fight from 
the Hidden Inventory Arc, where Gojo fights Toji. 
The winner of this fight is predetermined, so the only way to win,
is to fight at the level of the honored one.

Here’s a YouTube video of the fight this is based on
(it is a bit graphic, but it’s a cartoon):
https://www.youtube.com/watch?v=4Jt78n_A7jM

The game features:
 A white opponent (Toji) who moves randomly.
 Red and blue orbs that the player fires.
 A purple orb created when red and blue collide, which drifts toward
  the center and acts as an instant-kill attack.
 A health system, timer, HUD (Heads-Up Display), and special “Honored One” ending.

The goal of the game is to defeat Toji before time or ammo runs out.


FILES INCLUDED:
project.c - Main program source code
gfx.h - Graphics library header
gfx.o - Precompiled graphics object file
Makefile - Builds an executable named `project`
readme.txt - This file

GAME CONTROLS:


--- RED ORB (aimed shot) ---
 Hold A/D: rotate aiming arrow left/right
 Press R: fire the red orb in the aim direction

--- BLUE ORB (charged shot) ---
 Press B: begin charging (orb grows)
 Move mouse (optional): choose firing location
 Left-click: fire the blue orb with power based on charge time

--- GAME SYSTEM ---
• S: start game from title screen
• R: restart after win/lose
• Q: quit the game at any time

GAME MECHANICS:
• Toji begins with full health and moves randomly.
• Red and blue orbs deal damage proportional to their speed.
• If a red orb and a blue orb collide:
      a purple orb spawns
      it drifts toward the center
       instantly kills Toji if it touches him

• WIN conditions:
      - Toji’s health reaches 0
      - Purple orb kills Toji (triggers special “Honored One” ending)
(you can theoretically win by firing a bunch of red and blue shots)
(it’s a lot more efficient to time the purple shot)
(note: you get different endings depending on if you win with a purple shot or not)

• LOSE conditions:
      - Time reaches 60 seconds
      - Player fires more than 30 total shots

• HUD displays:
      - Blue charge gauge
      - Toji’s health bar
      - Timer
      - Shot counter
      - Instructions


NOTES FOR THE GRADER:
- No global variables are used except allowed constant macros.
- Structs, functions, modular design, and states are used extensively.
- All game logic is event-driven using gfx input and animation loops.
- I tried using gfx2.h, but the animation became a lot choppier for some reason,
  so, I switched back to gfx.h and it worked fine


