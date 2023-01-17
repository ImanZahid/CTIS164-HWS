/*
   CTIS164 - Template Source Program
----------
STUDENT : Iman Zahid 22101209
SECTION : 4
HOMEWORK: 1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: Press F2 for day/night.
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <gL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD 16 // Period for the timer.
#define TIMER_ON 1      // 0:disable timer, 1:enable timer

#define D2R 0.0174532

bool activetimer = false;
bool day = true;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
bool mode = false; // false = manual, true = automatic
bool showCharacter = false;
int winWidth, winHeight; // current Window width and height
int cx = 0, cy = 0;
int mouseX, mouseY;
bool autoGoUp = false, autoGoDown = false, autoGoLeft = false, autoGoRight = true;

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

//
// To display onto window using OpenGL commands
//

void displaybackground()
{
    // label

    glBegin(GL_QUADS);
    {
        glVertex2f(-135, 280);
        glVertex2f(135, 280);
        glVertex2f(135, 260);
        glVertex2f(-135, 260);
    }
    glEnd();
    glColor3ub(192, 192, 192);
    glBegin(GL_QUADS);
    {
        glVertex2f(-130, 275);
        glVertex2f(130, 275);
        glVertex2f(130, 263);
        glVertex2f(-130, 263);
    }
    glEnd();

    glColor3ub(0, 0, 0);
    if (showCharacter)
    {
        if (mode)
            vprint(-125, 250, GLUT_BITMAP_8_BY_13, "      Manual");
        else
            vprint(-125, 250, GLUT_BITMAP_8_BY_13, "      Autonomous");
        if (left || autoGoLeft)
            vprint(-125, 235, GLUT_BITMAP_8_BY_13, "      Direction: Left");
        else if (right || autoGoRight)
            vprint(-125, 235, GLUT_BITMAP_8_BY_13, "      Direction: Right");
        else if (up || autoGoUp)
            vprint(-125, 235, GLUT_BITMAP_8_BY_13, "      Direction: Up");
        else if (down || autoGoDown)
            vprint(-125, 235, GLUT_BITMAP_8_BY_13, "      Direction: Down");
        else
            vprint(-125, 235, GLUT_BITMAP_8_BY_13, "      Direction: Not Set");
    }
    else
    {
        vprint(-390, 270, GLUT_BITMAP_8_BY_13, "Click to show character");
    }

    vprint(-125, 265, GLUT_BITMAP_8_BY_13, "     IMAN ZAHID 22101209");
    vprint(-390, 250, GLUT_BITMAP_8_BY_13, "Press F1 to change mode");
    vprint(-390, 230, GLUT_BITMAP_8_BY_13, "Press F2 to change scene");

    // grass
    glColor3ub(0, 100, 0);
    glBegin(GL_QUADS);
    {
        glVertex2f(-400, -300);
        glVertex2f(-400, -50);
        glVertex2f(400, -50);
        glVertex2f(400, -300);
    }
    glEnd();

    // street lights
    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
    {
        glVertex2f(-200, -50);
        glVertex2f(-200, 150);
        glVertex2f(-190, 150);
        glVertex2f(-190, -50);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(-170, 130);
        glVertex2f(-200, 150);
        glVertex2f(-190, 150);
        glVertex2f(-160, 130);
    }
    glEnd();
    circle(-165, 130, 5);
    glBegin(GL_QUADS);
    {
        glVertex2f(200, -50);
        glVertex2f(200, 150);
        glVertex2f(190, 150);
        glVertex2f(190, -50);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(200, 150);
        glVertex2f(190, 150);
        glVertex2f(220, 130);
        glVertex2f(230, 130);
    }
    glEnd();
    circle(225, 130, 5);

    // street light on the other side of the street
    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
    {
        glVertex2f(0, -225);
        glVertex2f(0, -5);
        glVertex2f(-10, -5);
        glVertex2f(-10, -225);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(0, -5);
        glVertex2f(40, 20);
        glVertex2f(30, 20);
        glVertex2f(-10, -5);
    }
    glEnd();
    circle(34, 20, 6);

    // sun
    glColor3ub(255, 225, 0);
    circle(125, 175, 45);
}

// character information
void drawCharacter()
{
    // face
    glColor3ub(205, 127, 50);
    circle(0 + cx, 0 + cy, 30);
    glColor3f(1, 1, 1);
    circle(13 + cx, 15 + cy, 5);
    circle(-13 + cx, 15 + cy, 5);
    glColor3f(0, 0, 0);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(13 + cx, 14 + cy);
    glVertex2f(-13 + cx, 14 + cy);
    glEnd();
    glPointSize(1);
    glColor3ub(148, 116, 22);
    glBegin(GL_TRIANGLES);
    glVertex2f(-10 + cx, 5 + cy);
    glVertex2f(10 + cx, 5 + cy);
    glVertex2f(0 + cx, -8 + cy);
    glEnd();
    glColor3ub(213, 82, 17);
    glRectf(-20 + cx, -20 + cy, 20 + cx, -10 + cy);

    // body
    glColor3f(56 / 255.0, 118 / 255.0, 114 / 255.0);
    glRectf(-12 + cx, -30 + cy, 12 + cx, -70 + cy);

    // pants
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(-12 + cx, -70 + cy);
    glVertex2f(-18 + cx, -110 + cy);
    glVertex2f(-10 + cx, -110 + cy);
    glVertex2f(1 + cx, -85 + cy);
    glVertex2f(8 + cx, -110 + cy);
    glVertex2f(16 + cx, -110 + cy);
    glVertex2f(11 + cx, -70 + cy);
    glEnd();

    // shoes
    glColor3f(0, 0, 0);
    glRectf(-5 + cx, -110 + cy, -25 + cx, -115 + cy);
    glRectf(21 + cx, -110 + cy, 3 + cx, -115 + cy);

    // arms
    glColor3f(56 / 255.0, 118 / 255.0, 114 / 255.0);
    glBegin(GL_POLYGON);
    glVertex2f(-12 + cx, -40 + cy);
    glVertex2f(-20 + cx, -65 + cy);
    glVertex2f(-18 + cx, -70 + cy);
    glVertex2f(-12 + cx, -50 + cy);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(12 + cx, -40 + cy);
    glVertex2f(20 + cx, -65 + cy);
    glVertex2f(18 + cx, -70 + cy);
    glVertex2f(12 + cx, -50 + cy);
    glEnd();

    // wrists
    glColor3ub(219, 183, 64);
    glRectf(-20 + cx, -65 + cy, -18 + cx, -70 + cy);
    glRectf(20 + cx, -65 + cy, 18 + cx, -70 + cy);
}

// typedef struct {
//     //center of character
//     int x, y;
//     //if the character is moving
//     bool move;
// }character_t;

void night()
{

    // streetlight
    glColor4ub(41, 76, 132, 255);
    glBegin(GL_QUADS);
    {
        glVertex2f(-400, -50);
        glVertex2f(-400, 225);
        glVertex2f(400, 225);
        glVertex2f(400, -50);
    }
    glEnd();

    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
    {
        glVertex2f(-200, -50);
        glVertex2f(-200, 150);
        glVertex2f(-190, 150);
        glVertex2f(-190, -50);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(-170, 130);
        glVertex2f(-200, 150);
        glVertex2f(-190, 150);
        glVertex2f(-160, 130);
    }
    glEnd();
    circle(-165, 130, 5);
    glBegin(GL_QUADS);
    {
        glVertex2f(200, -50);
        glVertex2f(200, 150);
        glVertex2f(190, 150);
        glVertex2f(190, -50);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(200, 150);
        glVertex2f(190, 150);
        glVertex2f(220, 130);
        glVertex2f(230, 130);
    }
    glEnd();
    circle(225, 130, 5);

    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
    {
        glVertex2f(0, -225);
        glVertex2f(0, -5);
        glVertex2f(-10, -5);
        glVertex2f(-10, -225);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex2f(0, -5);
        glVertex2f(40, 20);
        glVertex2f(30, 20);
        glVertex2f(-10, -5);
    }
    glEnd();
    circle(34, 20, 6);

    // moon

    glColor3f(248 / 255.0, 248 / 255.0, 255 / 255.0);
    circle(125, 175, 45);
}

void display()
{
    //
    // clear window to black
    //
    glClearColor(253 / 255.0, 253 / 255.0, 150 / 255.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    displaybackground();
    if (!day)
        night();
    if (showCharacter)
        drawCharacter();

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
        autoGoUp = true;
        autoGoDown = false;
        autoGoRight = false;
        autoGoLeft = false;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        autoGoUp = false;
        autoGoDown = true;
        autoGoRight = false;
        autoGoLeft = false;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        autoGoUp = false;
        autoGoDown = false;
        autoGoRight = false;
        autoGoLeft = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        autoGoUp = false;
        autoGoDown = false;
        autoGoRight = true;
        autoGoLeft = false;
        break;
    }

    if (key == GLUT_KEY_F1)
    {
        mode = !mode;
    }

    if (key == GLUT_KEY_F2)
    {
        day = !day;
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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        if (!showCharacter)
        {
            showCharacter = true;
        }

        cx = mouseX;
        cy = mouseY;
    }

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
    mouseX = x - (winWidth / 2);
    mouseY = (winHeight / 2) - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v)
{

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    // bool mode = false;       // false = manual, true = automatic

    int widthBound = (WINDOW_WIDTH / 2) - 45;
    int heightBound = (WINDOW_HEIGHT / 2) - 45;

    if (!mode)
    { // automatic

        if (autoGoRight)
        {
            cx += 3;
            if (cx + 30 >= widthBound)
            {
                autoGoRight = false;
                autoGoLeft = true;
            }
        }
        else if (autoGoLeft)
        {
            cx -= 3;
            if (cx + 30 <= -widthBound)
            {
                autoGoLeft = false;
                autoGoRight = true;
            }
        }
        else if (autoGoUp)
        {
            cy += 3;
            if (cy + 30 >= heightBound - 75)
            {
                autoGoUp = false;
                autoGoDown = true;
            }
        }
        else if (autoGoDown)
        {
            cy -= 3;
            if (cy + 30 <= -heightBound + 75)
            {
                autoGoDown = false;
                autoGoUp = true;
            }
        }
    }
    else
    { // manual
        if (up)
        {
            if (cy + 30 <= heightBound - 35)
            {
                cy += 3;
            }
            down = false;
        }
        else if (down)
        {
            if (cy + 30 >= -heightBound + 110)
            {
                cy -= 3;
            }
            up = false;
        }
        else if (left)
        {
            if (cx + 15 >= -widthBound)
            {
                cx -= 3;
            }
            right = false;
        }
        else if (right)
        {
            if (cx <= widthBound)
            {
                cx += 3;
            }
            left = false;
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
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // glutInitWindowPosition(100, 100);
    glutCreateWindow("Flying Man");

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