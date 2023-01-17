/*********
   CTIS164 - Template Source Program
----------
STUDENT : Iman Zahid
SECTION : 4
HOMEWORK: 3
----------
PROBLEMS: If the time period is less than 50, the bird dissapears.
----------
ADDITIONAL FEATURES:
Random head colors
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <gL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"
#include <time.h>
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700
#define BIRD_RADIUS 30
#define TIMER_PERIOD  50 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define SPEED 5
#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
typedef struct
{
    vec_t bird;
    vec_t position;
    vec_t vel;
    double angle;

}Bird;
typedef struct
{

    double r, g, b;

}color;
color rgb;
vec_t Mousepos;
Bird enemy;
double TopX = -500;
double TopWidth = 1000;
double TopY = 300;
double TopHeight = 500;
bool visible = true;
bool pause = false;
double angle2 = 0;
vec_t lastpos={};
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
void vprint2(int x, int y, float size, const char* string, ...) {
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

//
// To display onto window using OpenGL commands
//
void vertex(vec_t P, vec_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}
void ChaseMouse()
{
   
    if (pause == false)
    {
        enemy.angle = atan2(Mousepos.y, Mousepos.x) / D2R;
        if (enemy.angle < 0)
        {
            enemy.angle += 360;
        }


        if (visible == true)
        {

            enemy.vel = mulV(SPEED, unitV(subV(Mousepos, enemy.position)));
            enemy.position = addV(enemy.position, enemy.vel);
            angle2 = enemy.angle;
            lastpos = enemy.position;
        }
        else
        {
            if (enemy.position.x  + BIRD_RADIUS + 20> TopX + TopWidth || enemy.position.x - BIRD_RADIUS -20< TopX  )
            {
                enemy.vel.x *= -1;

            }

            if (enemy.position.y + BIRD_RADIUS  + 20> TopY || enemy.position.y - BIRD_RADIUS -20 < TopY - TopHeight)
            {
                enemy.vel.y *= -1;

            }

            enemy.angle = angle2;
            enemy.position = addV(enemy.position, enemy.vel);
        }
       

    }


}
void drawBird()
{
    // bird
    double newangle = enemy.angle * D2R;
    
    glBegin(GL_TRIANGLES); // beak of the bird
    glColor3f(0.5, 0.5, 0.5);
    vertex({ enemy.bird.x + 0, enemy.bird.y + 0 }, enemy.position, newangle);
    vertex({ enemy.bird.x + 20, enemy.bird.y + 20 }, enemy.position, newangle);
    vertex({ enemy.bird.x + 60, enemy.bird.y + 10 }, enemy.position, newangle);
    glEnd();

    glColor3f(rgb.r/255.0, rgb.g/255.0, rgb.b/255.0);
    circle(enemy.bird.x + 0 + enemy.position.x , enemy.bird.y + 0 + enemy.position.y, BIRD_RADIUS); // head of the bird

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(3);
    circle_wire(enemy.bird.x + 0 + enemy.position.x, enemy.bird.y + 0 + enemy.position.y, BIRD_RADIUS);

    glColor3f(0.5, 0.5, 0.5);
    circle(enemy.bird.x - 10 + enemy.position.x, enemy.bird.y - 35 + enemy.position.y, BIRD_RADIUS); // body

    glColor3f(1, 1, 1);
    circle(enemy.bird.x + 5 + enemy.position.x, enemy.bird.y + 20 + enemy.position.y, 5); // eyes

    glColor3f(0, 0, 0);
    circle(enemy.bird.x + 5 + enemy.position.x, enemy.bird.y + 20 + enemy.position.y, 2);

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1);
    circle_wire(enemy.bird.x + 5 + enemy.position.x, enemy.bird.y + 20 + enemy.position.y, 5);

    glLineWidth(2);
    glBegin(GL_LINES); // legs
    glColor3f(0.5, 0.5, 0.5);
    vertex({ enemy.bird.x - 10, enemy.bird.y - 40 }, enemy.position, newangle);
    vertex({ enemy.bird.x - 20, enemy.bird.y - 85 }, enemy.position, newangle);
    vertex({ enemy.bird.x - 20, enemy.bird.y - 40 }, enemy.position, newangle);
    vertex({ enemy.bird.x - 40, enemy.bird.y - 80 }, enemy.position, newangle);
    glEnd();

    glColor3f(0, 0, 0);
    vprint(enemy.bird.x - 25 + enemy.position.x, enemy.bird.y - 35 + enemy.position.y, GLUT_BITMAP_8_BY_13, "BIRD");
}

void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    vprint(-20, 320, GLUT_BITMAP_HELVETICA_18, "IMAN ZAHID");

    glColor3f(0.8, 0.8, 0);
    glRectf(TopX, TopY, TopX+ TopWidth, TopY- TopHeight);
    drawBird();
    if (visible==true)
    {
        glColor3f(1, 0, 0);
        circle(Mousepos.x, Mousepos.y, 10);
        glColor3f(1, 1, 1);

        vprint(-400, -300, GLUT_BITMAP_HELVETICA_18, "MOUSE IS INSIDE");
    }
    else
    {
        glColor3f(1, 1, 1);

        vprint(-400, -300, GLUT_BITMAP_HELVETICA_18, "MOUSE IS OUTSIDE");


    }
    glColor3f(1, 1, 1);
    vprint(-140, -300, GLUT_BITMAP_HELVETICA_18, "PRESS SPACEBAR TO PAUSE ANIMATION");
    vprint(400, -300, GLUT_BITMAP_HELVETICA_18, "Angle: %0.0f", enemy.angle);

    glColor3f(0, 0, 0);
   

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

    if (key == ' ')
    {
        pause = !pause;

    }
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
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
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
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
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

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    Mousepos.x = x - winWidth / 2;
    Mousepos.y = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    ChaseMouse();
    rgb.r = rand() % 255;
    rgb.g = rand() % 200;
    rgb.b = rand() % 180;

    if (Mousepos.x > TopX && Mousepos.x < TopX + TopWidth && Mousepos.y  < TopY && Mousepos.y > TopY - TopHeight)

    {

        visible = true;
    }
    else
    {
        visible = false;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enemy.bird.x = 0;
    enemy.bird.y = 0;
    enemy.position.x = 0;
    enemy.position.y = 0;
    srand(time(NULL));
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Colourful Bird");

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

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}