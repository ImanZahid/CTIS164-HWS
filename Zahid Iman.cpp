/*
   CTIS164 - Template Source Program
----------
STUDENT : Iman Zahid
SECTION : 4
HOMEWORK: 2
----------
PROBLEMS: 
----------
ADDITIONAL FEATURES:
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// TODO: remove this include
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <cmath>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD 32//Period for the timer.
#define TIMER_ON 1      // 0:disable timer, 1:enable timer
#define TIMER_MAPPED_TO_60 31.25
#define D2R 0.0174532

#define LINE_SCALE 50

// Barrel
#define BARREL_CENTER_X 0
#define BARREL_CENTER_Y 0
#define BARREL_RADIUS 10
#define BARREL_ANGLE -90
#define BARREL_SPEED 20

// Bird
#define BIRD_RADIUS 30

// Structs
typedef struct
{
    float x;
    float y;
} Point;

typedef struct
{
    Point pos;
    float angle;
    float r;
} Barrel;

typedef struct
{
    Point pos;
    bool active;
} Fire;

typedef struct {
    float r;
    float g;
    float b;
} Color;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;

// current Window width and height
int winWidth = WINDOW_WIDTH;
int winHeight = WINDOW_HEIGHT;

Barrel barrel = { {BARREL_CENTER_X, BARREL_CENTER_Y}, BARREL_ANGLE, BARREL_SPEED };
Point bird = { 200, 200 };
Fire fire = { {0, 0}, false };
bool startGame = false;
float timerCounter = 60;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

bool hitTarget()
{
    return sqrtf(powf(fire.pos.x - bird.x, 2) + powf(fire.pos.y - bird.y, 2)) <= 50;
}

void reset()
{
    int absoluteWidth = winWidth / 2;
    int absoluteHeight = winHeight / 2;
    do {
        bird.x = (rand() % (absoluteWidth + absoluteWidth + 1)) - absoluteWidth;
    } while (abs(bird.x) > absoluteWidth - 100);
    do {

        bird.y = (rand() % (absoluteHeight + absoluteHeight + 1)) - absoluteHeight;

    } while (abs(bird.y) > absoluteHeight - 100);
}

bool isOnLineOfFire()
{
    Point lineStart = { barrel.pos.x, barrel.pos.y };
    Point lineEnd = { barrel.pos.x + LINE_SCALE * cos(barrel.angle * D2R), barrel.pos.y + LINE_SCALE * sin(barrel.angle * D2R) };
    float gradient = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x);
    float yIntercept = lineStart.y - gradient * lineStart.x;

    float barrelEndX = barrel.pos.x + LINE_SCALE * cos(barrel.angle * D2R);
    float barrelEndY = barrel.pos.y + LINE_SCALE * sin(barrel.angle * D2R);
    float barrelEndLengthFromOrigin = sqrtf(powf(barrelEndX, 2) + powf(barrelEndY, 2));
    float birdCenterFromOrigin = sqrtf(powf(bird.x, 2) + powf(bird.y, 2));
    float birdCenterFromBarrelEnd = sqrtf(powf(bird.x - barrelEndX, 2) + powf(bird.y - barrelEndY, 2));

    // barrel end is not in same direction as the bird
    if (abs(int(birdCenterFromOrigin + barrelEndLengthFromOrigin) - int(birdCenterFromBarrelEnd)) < 2)
    {
        return false;
    }
    if (abs(int(bird.y) - int(gradient * bird.x + yIntercept)) < 10)
    {
        return true;
    }
    return false;
}

void rotateBarrel()
{
    barrel.angle += 3;
    barrel.angle = fmod(barrel.angle, 360); // to make sure angle is between 0 and 360
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawBackground()
{
    // background colour
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex2d(-(winWidth / 2), (winHeight / 2));
    glVertex2d((winWidth / 2), (winHeight / 2));
    glColor3f(0, 1.0, 1.0);
    glVertex2d((winWidth / 2), -(winHeight / 2));
    glVertex2d(-(winWidth / 2), -(winHeight / 2));
    glEnd();

    // label

    glColor3f(0, 0, 255);
    vprint(-125, 250, GLUT_BITMAP_8_BY_13, "          HOMEWORK 2");
    vprint(-125, 265, GLUT_BITMAP_8_BY_13, "     IMAN ZAHID 22101209");
}

void drawFire()
{
    glColor3f(1, 1, 1);
    circle(fire.pos.x, fire.pos.y, 6);
}

void drawBarrel()
{
    glColor3f(1, 150 / 255.0, 0);
    glLineWidth(6);
    glBegin(GL_LINES);
    glVertex2f(barrel.pos.x, barrel.pos.y);
    glVertex2f(barrel.pos.x + LINE_SCALE * cos(barrel.angle * D2R), barrel.pos.y + LINE_SCALE * sin(barrel.angle * D2R));

    glEnd();
    glLineWidth(1);

    glColor3f(0.3, 0.3, 1.0);
    circle(barrel.pos.x, barrel.pos.y, barrel.r);
    glColor3f(0, 0, 0);
    circle(barrel.pos.x, barrel.pos.y, barrel.r - 4);

    glColor3f(1, 1, 1);
    vprint(barrel.pos.x - 12, barrel.pos.y - 5, GLUT_BITMAP_8_BY_13, "%.0f", barrel.angle);
}

void drawTimer() {
    glColor3f(0, 1, 0);
    glRectf(-winWidth / 2, -winHeight / 2, winWidth / 2, -winHeight / 2 + 60);
    glColor3f(1, 1, 1);
    vprint(-winWidth / 2 + 40, -winHeight / 2 + 20, GLUT_BITMAP_HELVETICA_18, "Timer : %.2f", timerCounter);
}

void drawBird()
{
    // bird
    glBegin(GL_TRIANGLES); // beak of the bird
    glColor3f(0.5, 0.5, 0.5);
    glVertex2f(bird.x + 0, bird.y + 0);
    glVertex2f(bird.x + 20, bird.y + 20);
    glVertex2f(bird.x + 60, bird.y + 10);
    glEnd();

    glColor3f(1, 1, 1);
    circle(bird.x + 0, bird.y + 0, BIRD_RADIUS); // head of the bird

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(3);
    circle_wire(bird.x + 0, bird.y + 0, BIRD_RADIUS);

    glColor3f(0.5, 0.5, 0.5);
    circle(bird.x - 10, bird.y - 35, BIRD_RADIUS); // body

    glColor3f(1, 1, 1);
    circle(bird.x + 5, bird.y + 20, 5); // eyes

    glColor3f(0, 0, 0);
    circle(bird.x + 5, bird.y + 20, 2);

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1);
    circle_wire(bird.x + 5, bird.y + 20, 5);

    glLineWidth(2);
    glBegin(GL_LINES); // legs
    glColor3f(0.5, 0.5, 0.5);
    glVertex2f(bird.x - 10, bird.y - 40);
    glVertex2f(bird.x - 20, bird.y - 85);
    glVertex2f(bird.x - 20, bird.y - 40);
    glVertex2f(bird.x - 40, bird.y - 80);
    glEnd();

    glColor3f(0, 0, 0);
    vprint(bird.x - 25, bird.y - 35, GLUT_BITMAP_8_BY_13, "BIRD");
}

//
// To display onto window using OpenGL commands
//
void display()
{
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    if (startGame)
    {
        if (fire.active)
        {
            drawFire();
        }
        drawBarrel();
        drawBird();
        drawTimer();
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key)
    {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key)
    {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    startGame = true;
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y)
{
    // Write your codes here.

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y)
{
    // Write your codes here.

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v)
{

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (startGame) {
        if (timerCounter > 0)
        {
            timerCounter -= 1 / (TIMER_MAPPED_TO_60);
        }
        else {
            startGame = false;
        }
    }
    if (isOnLineOfFire())
    {
        fire.active = true;
    }
    else
    {
        rotateBarrel();
    }
    if (fire.active)
    {
        if (hitTarget())
        {
            fire.active = false;
            fire.pos.x = 0;
            fire.pos.y = 0;
            reset();
        }
        else
        {
            fire.pos.x += 10 * cos(barrel.angle * D2R);
            fire.pos.y += 10 * sin(barrel.angle * D2R);

            if (fire.pos.x > (winWidth / 2) || fire.pos.x < -(winWidth / 2) || fire.pos.y >(winHeight / 2) || fire.pos.y < -(winHeight / 2))
            {
                fire.active = false;
            }
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()
}
#endif

void Init()
{

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    // glutInitWindowPosition(100, 100);
    glutCreateWindow("Kill The Bird");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();

    return 0;
}