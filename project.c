// Name: Nathaniel Mahadeo
// Assignment: Lab 11 - Mini Final Project (project.c)
//
// Controls:
//   RED:   aim with 'a' / 'd', fire with 'r'
//   BLUE:  press 'b' to charge, left click to fire
//   OTHER: 's' = start game, 'q' = quit, 'r' = restart after win/lose
//
// Behavior:
//   - Purple orb spawns when red and blue collide and drifts toward the center.
//   - Toji is a white opponent with a triangular torso that moves randomly.
//   - Toji has health; takes damage proportional to orb speed; purple orb is an insta-kill.
//   - You WIN by reducing Toji's health to zero.
//   - If you win with a purple kill, you see a special "Honored One" screen.
//   - You LOSE if time runs out or you run out of shots before defeating Toji.

#include "gfx.h"
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// known constant configuration values (allowed as macros)
#define windowWidth 600
#define windowHeight 500
#define tojiRadius 25.0f
#define timeLimit 60.0f      // seconds before you lose if Toji is still alive
#define frameDt 0.03f        // ~30 ms per frame
#define maxShots 30          // total combined red + blue shots before you lose

// struct definition for an orb (red, blue, purple)
typedef struct {
    float x, y;
    float dx, dy;
    float radius;
    int alive;
} Orb;

// struct definition for the Toji opponent
typedef struct {
    float x, y;
    float dx, dy;
    float speed;
    float health;
    float maxHealth;
    int alive;
} Toji;

// enum for high-level game state
typedef enum {
    stateStart,
    statePlaying,
    stateWin,
    stateWinHonored,   // special screen if purple orb gets the kill
    stateLose
} GameState;

// function prototypes for orb motion, collisions, input, HUD, and Toji behavior
void moveOrb(Orb *o);
void wallCollision(Orb *o);
void updateRed(Orb *red);
void updateBlue(Orb *blue);
void updatePurple(Orb *purple, float *ang, float spin, float speed);
void collision(Orb *red, Orb *blue, Orb *purple, float *ang);
void key(char c, int *run, Orb *red, float *aim, int *shotsFired);
void randomDirection(Orb *o, float speed);
float magnitude(float x, float y);
void updateToji(Toji *t);
void drawToji(Toji *t);
void tojiHitOrbs(Toji *t, Orb *red, Orb *blue, Orb *purple, int *honoredWin);
void drawY(double x, double y, double len, double angle);
void drawHUD(float chargeRatio, float healthRatio,
             int shotsFired, float elapsed);
void drawStartScreen(void);
void drawEndScreen(int win, int shotsFired, float elapsed);
void drawHonoredEndScreen(float gojoY, int shotsFired, float elapsed);

// helper to reset all game state at start/restart
void resetGame(Orb *red, Orb *blue, Orb *purple,
               Toji *toji,
               int *blueCharging, float *blueCharge,
               float *aimAngle, float *hAngle,
               int *frameCount, int *shotsFired,
               int *honoredWin)
{
    int baseRadius = 20;

    // RED orb
    red->x = 0;
    red->y = 0;
    red->dx = 0;
    red->dy = 0;
    red->radius = baseRadius;
    red->alive = 0;

    // BLUE orb
    blue->x = 0;
    blue->y = 0;
    blue->dx = 0;
    blue->dy = 0;
    blue->radius = baseRadius;
    blue->alive = 0;

    // PURPLE orb
    purple->x = 0;
    purple->y = 0;
    purple->dx = 0;
    purple->dy = 0;
    purple->radius = 25.0f;
    purple->alive = 0;

    // charging and aim
    *blueCharging = 0;
    *blueCharge = 0;
    *aimAngle = 0;
    *hAngle = 0;

    // timer and shots
    *frameCount = 0;
    *shotsFired = 0;

    // honored win flag (purple kill) cleared
    *honoredWin = 0;

    // Toji in center with random direction
    toji->x = windowWidth / 2.0f;
    toji->y = windowHeight / 2.0f;
    toji->speed = 3.0f;
    toji->maxHealth = 100;
    toji->health = toji->maxHealth;
    toji->alive = 1;
    toji->dx = ((rand() % 2001) - 1000) / 1000.0f * toji->speed;
    toji->dy = ((rand() % 2001) - 1000) / 1000.0f * toji->speed;
}

int main()
{
    Orb red, blue, purple;
    Toji toji;

    // blue charging state and charge parameters
    int blueCharging = 0;
    float blueCharge = 0, blueChargeMax = 40;
    float chargeRate = 1;
    float xbCharge = 0, ybCharge = 0;

    // aiming and purple rotation
    float aimAngle = 0;
    float hAngle = 0;
    float hSpin = 0.12f;
    float hSpeed = 3.0f;

    // timer and shot counter
    int frameCount = 0;
    int shotsFired = 0;

    // flag: did the player win via purple insta-kill?
    int honoredWin = 0;

    // y-position for Gojo sprite on honored win screen (lying down)
    float gojoY = windowHeight + 80.0f;

    // open the gfx window
    gfx_open(windowWidth, windowHeight, "Gojo vs Toji");
    int running = 1;

    GameState state = stateStart;

    // initialize game state for when we transition into PLAYING
    resetGame(&red, &blue, &purple, &toji,
              &blueCharging, &blueCharge,
              &aimAngle, &hAngle,
              &frameCount, &shotsFired,
              &honoredWin);

    // main loop
    while (running) {
        gfx_clear();

        if (state == stateStart) {
            // draw start screen with title and instructions
            drawStartScreen();
            gfx_flush();

            if (gfx_event_waiting()) {
                char c = gfx_wait();
                if (c == 'q') {
                    running = 0;
                } else if (c == 's') {
                    // reset and start playing
                    resetGame(&red, &blue, &purple, &toji,
                              &blueCharging, &blueCharge,
                              &aimAngle, &hAngle,
                              &frameCount, &shotsFired,
                              &honoredWin);
                    state = statePlaying;
                }
            }

            usleep(30000);
            continue;
        }

        if (state == stateWinHonored) {
            // special honored-one end screen with Gojo sprite
            float elapsed = frameCount * frameDt;

            drawHonoredEndScreen(gojoY, shotsFired, elapsed);
            gfx_flush();

            // slowly move Gojo upward until he reaches near the upper-middle
            if (gojoY > windowHeight / 2.0f - 120.0f) {
                gojoY -= 1.0f;
            }

            if (gfx_event_waiting()) {
                char c = gfx_wait();
                if (c == 'q') {
                    running = 0;
                } else if (c == 'r') {
                    resetGame(&red, &blue, &purple, &toji,
                              &blueCharging, &blueCharge,
                              &aimAngle, &hAngle,
                              &frameCount, &shotsFired,
                              &honoredWin);
                    gojoY = windowHeight + 80.0f;
                    state = statePlaying;
                }
            }

            usleep(30000);
            continue;
        }

        if (state == stateWin || state == stateLose) {
            // regular win/lose screen
            float elapsed = frameCount * frameDt;
            drawEndScreen(state == stateWin, shotsFired, elapsed);
            gfx_flush();

            if (gfx_event_waiting()) {
                char c = gfx_wait();
                if (c == 'q') {
                    running = 0;
                } else if (c == 'r') {
                    resetGame(&red, &blue, &purple, &toji,
                              &blueCharging, &blueCharge,
                              &aimAngle, &hAngle,
                              &frameCount, &shotsFired,
                              &honoredWin);
                    gojoY = windowHeight + 80.0f;
                    state = statePlaying;
                }
            }

            usleep(30000);
            continue;
        }

        // statePlaying: main game logic

        int mx = gfx_xpos(), my = gfx_ypos();

        // compute fractions for HUD
        float chargeRatio = blueCharge / blueChargeMax;
        if (!blueCharging && !blue.alive) chargeRatio = 0;
        float healthRatio = toji.alive ? (toji.health / toji.maxHealth) : 0;
        float elapsed = frameCount * frameDt;

        // draw HUD (bars + timer + shots)
        drawHUD(chargeRatio, healthRatio, shotsFired, elapsed);

        // draw the red aiming arrow (Y-shape) at mouse position
        gfx_color(255, 0, 0);
        drawY(mx, my, 60, aimAngle);

        // update and draw the bouncing blue orb (if alive)
        updateBlue(&blue);

        // draw charging circle while blue is charging but not yet fired
        if (blueCharging && !blue.alive) {
            blueCharge += chargeRate;
            if (blueCharge > blueChargeMax) blueCharge = blueChargeMax;
            gfx_color(0, 0, 255);
            gfx_circle(xbCharge, ybCharge, blue.radius + blueCharge);
        }

        // update and draw the red orb (no bounce, despawns offscreen)
        updateRed(&red);

        // update and draw the purple orb that drifts toward center
        updatePurple(&purple, &hAngle, hSpin, hSpeed);

        // handle collision between red and blue (spawns purple)
        collision(&red, &blue, &purple, &hAngle);

        // move Toji and keep him below HUD area
        updateToji(&toji);

        // check if any orbs hit Toji and apply damage/insta-kill
        if (toji.alive) {
            tojiHitOrbs(&toji, &red, &blue, &purple, &honoredWin);
        }

        // draw Toji if still alive
        drawToji(&toji);

        // show everything on screen
        gfx_flush();

        // check for keyboard/mouse input
        if (gfx_event_waiting()) {
            char c = gfx_wait();

            // handle aiming and firing red orb, quit, etc.
            key(c, &running, &red, &aimAngle, &shotsFired);

            // start charging blue orb when 'b' is pressed
            if (c == 'b') {
                blueCharging = 1;
                blueCharge = 0;
                xbCharge = gfx_xpos();
                ybCharge = gfx_ypos();
                blue.x = xbCharge;
                blue.y = ybCharge;
                blue.dx = 0;
                blue.dy = 0;
                blue.alive = 0;
            }

            // left-click (mouse button 1) fires the charged blue orb
            if (c == 1 && blueCharging && !blue.alive) {
                blueCharging = 0;
                blue.alive = 1;
                float t = blueCharge / blueChargeMax;
                float speed = 2 + t * (10 - 2);
                randomDirection(&blue, speed);
                shotsFired++;
            }
        }

        // advance timer
        frameCount++;

        // check win condition (distinguish honored vs regular)
        if (!toji.alive) {
            if (honoredWin) {
                // special honored-one win via purple kill
                gojoY = windowHeight + 80.0f;
                state = stateWinHonored;
            } else {
                state = stateWin;
            }
        }
        // check lose conditions (time or ammo)
        else if ((elapsed >= timeLimit) || (shotsFired >= maxShots)) {
            state = stateLose;
        }

        // short delay to control animation speed
        usleep((useconds_t)(frameDt * 1000000));
    }

    return 0;
}

// move an orb by its velocity
void moveOrb(Orb *o)
{
    o->x += o->dx;
    o->y += o->dy;
}

// bounce an orb off the window walls
void wallCollision(Orb *o)
{
    if (o->x + o->radius >= windowWidth || o->x - o->radius <= 0) o->dx = -o->dx;
    if (o->y + o->radius >= windowHeight  || o->y - o->radius <= 0) o->dy = -o->dy;
}

// compute Euclidean length of a 2D vector
float magnitude(float x, float y)
{
    return sqrt(x*x + y*y);
}

// choose a random direction for an orb with a given speed
void randomDirection(Orb *o, float speed)
{
    float a = ((rand() % 2001) - 1000) / 1000.0f;
    float b = ((rand() % 2001) - 1000) / 1000.0f;
    float len = magnitude(a, b);
    if (len == 0) len = 1;
    o->dx = speed * (a / len);
    o->dy = speed * (b / len);
}

// draw and update the red orb; despawn when it leaves the screen
void updateRed(Orb *red)
{
    if (!red->alive) return;

    gfx_color(255, 0, 0);
    gfx_circle(red->x, red->y, red->radius);

    moveOrb(red);

    // if red orb goes completely offscreen, deactivate it
    if (red->x + red->radius < 0 || red->x - red->radius > windowWidth ||
        red->y + red->radius < 0 || red->y - red->radius > windowHeight)
        red->alive = 0;
}

// draw and update the bouncing blue orb
void updateBlue(Orb *blue)
{
    if (!blue->alive) return;

    gfx_color(0, 0, 255);
    gfx_circle(blue->x, blue->y, blue->radius);

    moveOrb(blue);
    wallCollision(blue);
}

// update the purple orb so it drifts toward the center of the screen and spins a line
void updatePurple(Orb *purple, float *ang, float spin, float speed)
{
    if (!purple->alive) return;

    float cx = windowWidth / 2.0f, cy = windowHeight / 2.0f;
    float dx = cx - purple->x, dy = cy - purple->y;
    float dist = magnitude(dx, dy);

    // once close to center, remove the purple orb
    if (dist < 5) {
        purple->alive = 0;
        return;
    }

    // move toward center and spin the line
    purple->x += speed * dx / dist;
    purple->y += speed * dy / dist;
    *ang += spin;

    gfx_color(255, 0, 255);
    gfx_circle(purple->x, purple->y, purple->radius);

    // rotating line inside the purple orb
    int lx = (int)(purple->x + purple->radius * cos(*ang));
    int ly = (int)(purple->y + purple->radius * sin(*ang));
    gfx_line((int)purple->x, (int)purple->y, lx, ly);
}

// detect collision between red and blue orbs and spawn purple orb
void collision(Orb *red, Orb *blue, Orb *purple, float *ang)
{
    // only check if red and blue are alive and purple isn't already active
    if (!red->alive || !blue->alive || purple->alive) return;

    float dx = red->x - blue->x;
    float dy = red->y - blue->y;

    // simple circle-circle collision: distance^2 <= (r1 + r2)^2
    float rSum = red->radius + blue->radius;
    if (dx*dx + dy*dy <= rSum * rSum) {
        red->alive = 0;
        blue->alive = 0;
        purple->alive = 1;
        purple->x = (red->x + blue->x) / 2.0f;
        purple->y = (red->y + blue->y) / 2.0f;
        purple->dx = 0;
        purple->dy = 0;
        purple->radius = 25.0f;
        *ang = 0;
    }
}

// handle key presses for quitting and firing the red orb
void key(char c, int *run, Orb *red, float *aim, int *shotsFired)
{
    // quit program
    if (c == 'q') *run = 0;

    // fire red orb in direction of current aim
    else if (c == 'r') {
        red->alive = 1;
        red->x = gfx_xpos();
        red->y = gfx_ypos();
        float speed = 8;
        red->dx = speed * cos(*aim);
        red->dy = speed * sin(*aim);
        red->radius = 20;
        (*shotsFired)++;
    }
    // rotate aim left
    else if (c == 'a') {
        *aim -= 0.1f;
    }
    // rotate aim right
    else if (c == 'd') {
        *aim += 0.1f;
    }
}

// update Toji's position, keeping him out of the HUD at the top
void updateToji(Toji *t)
{
    if (!t->alive) return;

    // move by current velocity
    t->x += t->dx;
    t->y += t->dy;

    // y-value for bottom of the HUD area (health bar + labels)
    const float TOP_BOUND = 60.0f;

    // bounce off left/right edges
    if (t->x <= 0 || t->x >= windowWidth)
        t->dx = -t->dx;

    // bounce off bottom edge
    if (t->y >= windowHeight)
        t->dy = -t->dy;

    // prevent Toji from entering the HUD region at the top
    if (t->y <= TOP_BOUND) {
        t->y = TOP_BOUND;
        if (t->dy < 0) t->dy = -t->dy;  // force him to move downward
    }

    // occasionally randomize direction while keeping same speed
    if (rand() % 70 == 0) {
        float a = ((rand() % 2001) - 1000) / 1000.0f;
        float b = ((rand() % 2001) - 1000) / 1000.0f;
        float len = magnitude(a, b);
        if (len == 0) len = 1;
        t->dx = t->speed * (a / len);
        t->dy = t->speed * (b / len);
    }

    // if health is depleted, mark Toji as dead
    if (t->health <= 0)
        t->alive = 0;
}

// draw Toji as a white figure with triangular torso, head, arms, and legs
void drawToji(Toji *t)
{
    if (!t->alive) return;

    gfx_color(255, 255, 255);

    // head
    gfx_circle(t->x, t->y - 14, 7);

    // torso triangle
    int sl = (int)t->x - 14;
    int sr = (int)t->x + 14;
    int sy = (int)t->y - 2;
    int wx = (int)t->x;
    int wy = (int)t->y + 20;

    gfx_line(sl, sy, sr, sy);
    gfx_line(sl, sy, wx, wy);
    gfx_line(sr, sy, wx, wy);

    // arms
    gfx_line((int)t->x, (int)t->y + 2, (int)t->x - 12, (int)t->y + 5);
    gfx_line((int)t->x, (int)t->y + 2, (int)t->x + 12, (int)t->y + 5);

    // legs
    gfx_line(wx, wy, wx - 8, wy + 18);
    gfx_line(wx, wy, wx + 8, wy + 18);

    // extra line (weapon or accent)
    gfx_line((int)t->x + 12, (int)t->y + 5, (int)t->x + 30, (int)t->y - 8);
}

// check collisions between Toji and all orbs, and apply damage/insta-kill
// honoredWin is set to 1 if the killing blow is from the purple orb
void tojiHitOrbs(Toji *t, Orb *red, Orb *blue, Orb *purple, int *honoredWin)
{
    if (!t->alive) return;

    // red orb deals damage based on its speed
    if (red->alive) {
        float dx = red->x - t->x;
        float dy = red->y - t->y;
        float rSum = tojiRadius + red->radius;
        if (dx*dx + dy*dy <= rSum * rSum) {
            t->health -= magnitude(red->dx, red->dy) * 0.5f;
            red->alive = 0;
        }
    }

    // blue orb also deals damage based on its speed
    if (blue->alive) {
        float dx = blue->x - t->x;
        float dy = blue->y - t->y;
        float rSum = tojiRadius + blue->radius;
        if (dx*dx + dy*dy <= rSum * rSum) {
            t->health -= magnitude(blue->dx, blue->dy) * 0.5f;
            blue->alive = 0;
        }
    }

    // purple orb is an insta-kill when it hits Toji
    if (purple->alive) {
        float dx = purple->x - t->x;
        float dy = purple->y - t->y;
        float rSum = tojiRadius + purple->radius;
        if (dx*dx + dy*dy <= rSum * rSum) {
            t->health = 0;
            t->alive = 0;
            purple->alive = 0;
            if (honoredWin != NULL) *honoredWin = 1; // mark special win condition
        }
    }
}

// draw a Y-shaped arrow used for aiming the red orb
void drawY(double x, double y, double len, double angle)
{
    // end of the main trunk
    double x2 = x + len * cos(angle);
    double y2 = y + len * sin(angle);

    // draw the main trunk
    gfx_line((int)x, (int)y, (int)x2, (int)y2);

    // length of side branches and angle offset
    double b = 0.6 * len;
    double d = M_PI / 6;

    // left branch
    gfx_line((int)x2, (int)y2,
             (int)(x2 + b * cos(angle - d)),
             (int)(y2 + b * sin(angle - d)));

    // right branch
    gfx_line((int)x2, (int)y2,
             (int)(x2 + b * cos(angle + d)),
             (int)(y2 + b * sin(angle + d)));
}

// draw the HUD: blue charge, Toji health, time, and shots
void drawHUD(float chargeRatio, float healthRatio,
             int shotsFired, float elapsed)
{
    // BLUE charge bar (top left)
    int barW  = 160, barH  = 10;
    int barX  = 10,  barY  = 20;
    int panelX = barX - 20, panelY = barY - 20;
    int panelW = barW + 40, panelH = 50;

    // Blue charge panel
    gfx_color(255, 255, 255);
    for (int y = panelY; y <= panelY + panelH; y++)
        gfx_line(panelX, y, panelX + panelW, y);

    gfx_color(0, 0, 0);
    gfx_text(barX, barY - 2, "Blue Charge");

    gfx_line(barX, barY, barX + barW, barY);
    gfx_line(barX, barY + barH, barX + barW, barY + barH);
    gfx_line(barX, barY, barX, barY + barH);
    gfx_line(barX + barW, barY, barX + barW, barY + barH);

    gfx_color(0, 0, 255);
    int fillW = (int)(chargeRatio * barW);
    for (int x = 0; x < fillW; x++)
        gfx_line(barX + x, barY + 1, barX + x, barY + barH - 1);

    // TOJI health bar (top right)
    int barW2  = 160, barH2  = 10;
    int barX2  = windowWidth - barW2 - 10;
    int barY2  = 20;
    int panelX2 = barX2 - 20, panelY2 = barY2 - 20;
    int panelW2 = barW2 + 40, panelH2 = 50;

    gfx_color(255, 255, 255);
    for (int y = panelY2; y <= panelY2 + panelH2; y++)
        gfx_line(panelX2, y, panelX2 + panelW2, y);

    gfx_color(0, 0, 0);
    gfx_text(barX2, barY2 - 2, "Toji Health");

    gfx_line(barX2, barY2, barX2 + barW2, barY2);
    gfx_line(barX2, barY2 + barH2, barX2 + barW2, barY2 + barH2);
    gfx_line(barX2, barY2, barX2, barY2 + barH2);
    gfx_line(barX2 + barW2, barY2, barX2 + barW2, barY2 + barH2);

    gfx_color(255, 0, 0);
    int fillW2 = (int)(healthRatio * barW2);
    for (int x = 0; x < fillW2; x++)
        gfx_line(barX2 + x, barY2 + 1, barX2 + x, barY2 + barH2 - 1);

    // instructions centered at top
    gfx_color(255, 255, 255);
    int instrY = 15;
    gfx_text(windowWidth/2 - 105, instrY,
             "b = charge blue   click = fire blue");
    gfx_text(windowWidth/2 - 105, instrY + 15,
             "a/d = aim red     r = fire red");

    // timer and shots info at bottom
    char buf[64];
    snprintf(buf, sizeof(buf), "Time: %.1f s  (Limit: %.0f s)", elapsed, timeLimit);
    gfx_text(20, windowHeight - 30, buf);

    snprintf(buf, sizeof(buf), "Shots: %d / %d", shotsFired, maxShots);
    gfx_text(20, windowHeight - 15, buf);
}

// draw the start screen with title and key instructions
void drawStartScreen(void)
{
    gfx_color(255, 255, 255);
    gfx_text(windowWidth/2 - 70, windowHeight/2 - 40, "Gojo vs Toji");
    gfx_text(windowWidth/2 - 130, windowHeight/2 - 10,
             "Press 's' to start, 'q' to quit");

    gfx_text(windowWidth/2 - 180, windowHeight/2 + 20,
             "Controls: a/d aim red, r fire red,");
    gfx_text(windowWidth/2 - 180, windowHeight/2 + 35,
             "          b charge blue, click fire blue");
}

// draw the regular end screen (win or lose) and show stats
void drawEndScreen(int win, int shotsFired, float elapsed)
{
    gfx_color(255, 255, 255);

    if (win) {
        gfx_text(windowWidth/2 - 40, windowHeight/2 - 20, "YOU WIN!");
    } else {
        gfx_text(windowWidth/2 - 45, windowHeight/2 - 20, "YOU LOSE");
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "Time: %.1f s", elapsed);
    gfx_text(windowWidth/2 - 40, windowHeight/2 + 5, buf);

    snprintf(buf, sizeof(buf), "Shots fired: %d", shotsFired);
    gfx_text(windowWidth/2 - 55, windowHeight/2 + 20, buf);

    gfx_text(windowWidth/2 - 110, windowHeight/2 + 45,
             "Press 'r' to restart, 'q' to quit");
}

// draw a special honored-one win screen with a Gojo sprite lying down & rising
void drawHonoredEndScreen(float gojoY, int shotsFired, float elapsed)
{
    gfx_color(255, 255, 255);

    // main text
    gfx_text(windowWidth/2 - 65, windowHeight/2 - 60, "YOU ARE THE HONORED ONE");
    gfx_text(windowWidth/2 - 125, windowHeight/2 - 40,
             "Your purple strike erased Toji in an instant.");

    char buf[64];
    snprintf(buf, sizeof(buf), "Time: %.1f s", elapsed);
    gfx_text(windowWidth/2 - 40, windowHeight/2 - 15, buf);

    snprintf(buf, sizeof(buf), "Shots fired: %d", shotsFired);
    gfx_text(windowWidth/2 - 55, windowHeight/2 + 0, buf);

    gfx_text(windowWidth/2 - 155, windowHeight/2 + 25,
             "Press 'r' to restart your domain, 'q' to leave.");

    // --- Gojo sprite, lying horizontally and rising ---

    float cx = windowWidth / 2.0f;
    float y  = gojoY;

    // Head (circle)
    int headX = (int)(cx - 35);
    int headY = (int)y;
    gfx_circle(headX, headY, 10);

    // Vertical blindfold (straight top-to-bottom line)
    gfx_line(headX, headY - 10, headX, headY + 10);

    // Torso: connected directly to the head edge (head radius = 10 → head edge at headX + 10)
    int bodyStartX = headX + 10;      // connects to right edge of head
    int bodyEndX   = (int)(cx + 25);
    int bodyY      = headY;

    gfx_line(bodyStartX, bodyY, bodyEndX, bodyY);

    // Arms attached to torso midpoint
    int midX = (bodyStartX + bodyEndX) / 2;
    gfx_line(midX, bodyY, midX + 12, bodyY - 8);  // top arm
    gfx_line(midX, bodyY, midX + 12, bodyY + 8);  // bottom arm

    // Legs at torso end
    gfx_line(bodyEndX, bodyY, bodyEndX + 15, bodyY - 10);
    gfx_line(bodyEndX, bodyY, bodyEndX + 15, bodyY + 10);
}

