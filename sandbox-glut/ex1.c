#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#endif
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

typedef struct Point {
  int x, y;
} Point;

typedef struct Angle {
  double x, y, z;
} Angle;

typedef enum State {
  None,
  Grab,
  GrabLeft,
} State;

static int   xsize = 300;
static int   ysize = 300;

static void  draw(void);
static void  drawDebugLine(void);
static void  draw1( double angle );
static void  displayFunc(void);
static void  reshapeFunc(int width, int height);
static void  keyboardFunc(unsigned char key, int x, int y);
static void  idleFunc(void);
static void  mouseFunc(int button, int state, int x, int y);
static void  motionFunc(int x, int y);
static void  passiveMotionFunc(int x, int y);

static Point mousePos = {0.0, 0.0};
static Point mouseLeft = {0.0, 0.0};
static State nowState = None;
static Angle worldRotate = {0.0, 0.0, 0.0};
static Angle teapodRotate = {0.0, 0.0, 0.0};

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(xsize, ysize);
    glutCreateWindow("sample");

    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc);
    glutKeyboardFunc(keyboardFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);

    glutMainLoop();
}

static void  displayFunc(void)
{
  printf("displayFunc");
  draw();
}

static void  reshapeFunc(int width, int height)
{
    printf("reshapeFunc: (%3d,%3d)\n",
           width,
           height);
    xsize = width;
    ysize = height;
}

static void  keyboardFunc(unsigned char key, int x, int y)
{
    printf("keyFunc: %d (%3d,%3d)\n", key, x, y);
    if( key == 'q' ) exit(0);
}

static void  idleFunc(void)
{
    draw();
}

static void  mouseFunc(int button, int state, int x, int y)
{
    switch(button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        printf("doun\n");
        mousePos.x = x;
        mousePos.y = y;
        nowState = Grab;
      }
      if (state == GLUT_UP) {
        printf("up\n");
        nowState = None;
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN) {
        printf("doun(R)\n");
        mouseLeft.x = x;
        mouseLeft.y = y;
        nowState = GrabLeft;
      }
      if (state == GLUT_UP) {
        printf("up\n");
        nowState = None;
      }
    }
}

static void  motionFunc(int x, int y)
{
  printf("motionFunc: (%3d,%3d)\n", x, y);
  if (nowState == Grab) {
    Angle moved = {0.0, 0.0, 0.0};
    moved.x = (double)(x - mousePos.x) / 5.0;
    moved.y = (double)(y - mousePos.y) / 5.0;
    worldRotate.x += moved.x;
    worldRotate.y += moved.y;
    mousePos.x = x;
    mousePos.y = y;
  } else if (nowState == GrabLeft) {
    Angle moved = {0.0, 0.0, 0.0};
    moved.x = (double)(x - mouseLeft.x) / 5.0;
    moved.y = (double)(y - mouseLeft.y) / 5.0;
    teapodRotate.x += moved.x;
    teapodRotate.y += moved.y;
    mouseLeft.x = x;
    mouseLeft.y = y;
  }
  glutPostRedisplay();
}

static void  passiveMotionFunc(int x, int y)
{
    printf("passiveMotionFunc: (%3d,%3d)\n", x, y);
}

static void  draw(void)
{
    static float  angle = 0.0;
    static float  angle2 = 0.0;
    float  ambiColor[4] = {0.0, 0.0, 0.0, 0.0};
    float  lightPos[4] = {200.0, -200.0, 500.0, 1.0};
    float  lightAmbi[4] = {0.2, 0.2, 0.2, 0.0};
    float  lightDiff[4] = {1.0, 1.0, 1.0, 0.0};
    float  lightSpec[4] = {1.0, 1.0, 1.0, 0.0};

    glClearColor( 0.7, 0.7, 0.7, 0.0 );
    glClearDepth( 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport( 0, 0, xsize, ysize );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 50.0, (double)xsize/(double)ysize, 500.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt( 500.0, 500.0, 500.0,
               0.0, 0.0, 0.0,
               0.0, 0.0, 1.0 );

    glRotated(worldRotate.x, 0, 0, 1);
    glRotated(worldRotate.y, 0, 1, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable( GL_LIGHTING );
    glShadeModel(GL_SMOOTH);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiColor);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glPushMatrix();
    glRotatef( -angle, 0, 0, 1 );
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbi );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiff );
    glLightfv( GL_LIGHT0, GL_SPECULAR, lightSpec );
    glPopMatrix();

    draw1( angle );
    drawDebugLine();
    glDisable( GL_LIGHTING );

    glutSwapBuffers();
}

static void  draw1( double angle )
{
    float   diffColor[4] = {1.0, 1.0, 0.0, 1.0};
    float   specColor[4] = {1.0, 1.0, 1.0, 1.0};
    float   diffColor2[4] = {0.0, 1.0, 0.0, 1.0};
    float   specColor2[4] = {1.0, 1.0, 1.0, 1.0};
    float   diffColor3[4] = {0.0, 0.0, 1.0, 1.0};
    float   specColor3[4] = {1.0, 1.0, 1.0, 1.0};

    glPushMatrix();
    glFrontFace(GL_CCW);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffColor );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specColor );
    glMaterialf( GL_FRONT, GL_SHININESS, 40.0 );
    glScalef(10.0, 10.0, 10.0);
    glEnable(GL_NORMALIZE);
    glBegin(GL_QUADS);
        glNormal3f( 0.0, 0.0, 0.2 );
        glVertex3f( -20.0, -20.0, 0.0 );
        glNormal3f( 0.0, 0.0, 0.2 );
        glVertex3f(  20.0, -20.0, 0.0 );
        glNormal3f( 0.0, 0.0, 0.2 );
        glVertex3f(  20.0,  20.0, 0.0 );
        glNormal3f( 0.0, 0.0, 0.2 );
        glVertex3f( -20.0,  20.0, 0.0 );
    glEnd();
    glDisable(GL_NORMALIZE);
    glPopMatrix();

    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffColor2 );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specColor2 );
    glMaterialf( GL_FRONT, GL_SHININESS, 10.0 );

    glPushMatrix();
    glTranslatef( 50.0, 50.0, 30.0 );
    glRotatef( angle+30.0, 0.0, 0.0, 1.0 );
    glFrontFace(GL_CCW);
    glutSolidCube( 60.0 );
    glPopMatrix();

    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffColor3 );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specColor3 );
    glMaterialf( GL_FRONT, GL_SHININESS, 10.0 );

    glPushMatrix();
    glTranslatef( -150.0, -50.0, 0.0 );
    glRotatef( teapodRotate.y, 0.0, 1.0, 0.0 );
    glRotatef( teapodRotate.x, 0.0, 0.0, 1.0 );
    glTranslatef( 0.0, 0.0, 75.0 );
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    glFrontFace(GL_CW);
    glutSolidTeapot( 100.0 );
    glPopMatrix();

}

static void drawDebugLine()
{
  glBegin(GL_LINES);
  glColor3d(0, 1, 0);
  glVertex2d(-1000, 0);
  glVertex2d( 1000, 0);

  glColor3d(1, 0, 0);
  glVertex2d(0, -1000);
  glVertex2d(0, 1000);

  glColor3d(0, 0, 1);
  glVertex3d(0, 0, -1000);
  glVertex3d(0, 0, 1000);
  glEnd();
}
