#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <CoreGraphics/CoreGraphics.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <GLUT/glut.h>

#include "tui.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void init();
void displayHandler(void);
void reshapeHandler(int, int);
void mouseMoveHandler(int, int);
void mouseClickHandler(int, int, int, int);
void idleHandler(void);
void keyDownHandler(unsigned char, int, int);
void keyUpHandler(unsigned char, int, int);
void mouseEntryHandler(int);
void drawCursor(void);

void setOrthographic(int, int);
void setPerspective(int, int);

static void setViewport(int, int);

static float gSpin = 0.f;

#define V 0.5f

static float vertices[] = {
	 V,  V,  V,
	 V, -V,  V,
	-V, -V,  V,
	-V,  V,  V,
	 V,  V, -V,
	 V, -V, -V,
	-V, -V, -V,
	-V,  V, -V
};

static float colors[] = {
	0.f, 0.f, 0.f,
	0.f, 0.f, 1.f,
	0.f, 1.f, 0.f,
	0.f, 1.f, 1.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 1.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 1.f
};

static unsigned int indices[] = {
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 1, 5, 4,
	0, 3, 2, 1,
	0, 4, 7, 3,
	2, 3, 7, 6
};

int main(int argc, char** argv) {
	printf("TinyUI Demo\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tiny UI Demo");
	
	int version = gladLoaderLoadGL();
	if (version == 0) {
		printf("Cannot load GL!\n");

		return -1;
	}

	printf("OpenGL %d.%d\n",
			GLAD_VERSION_MAJOR(version), 
			GLAD_VERSION_MINOR(version));

	glutDisplayFunc(displayHandler);
	glutReshapeFunc(reshapeHandler);
	glutIdleFunc(idleHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutMotionFunc(mouseMoveHandler);
	glutKeyboardFunc(keyDownHandler);
	glutKeyboardUpFunc(keyUpHandler);
	glutEntryFunc(mouseEntryHandler);

	init();
	tuiInit();

	glutMainLoop();

	return 0;
}

void init(void) {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#ifdef __APPLE__
	CGDisplayHideCursor(CGMainDisplayID());
#else
	glutSetCursor(GLUT_CURSOR_NONE);
#endif
}

void displayHandler(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	setViewport(windowWidth, windowHeight);

	setPerspective(windowWidth, windowHeight);

	glPushMatrix();
	glRotatef(gSpin, 1.f, 2.f, 3.f);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);

	glPopMatrix();
	
	setOrthographic(windowWidth, windowHeight);

	tuiBegin();

	if (tuiButton("Click me!", 10, 10, 100, 30)) {
		printf("Button clicked!\n");
	}

	if (tuiButton("Click me too!", 10, 50, 150, 40)) {
		printf("Other button clicked!\n");
	}

	tuiEnd();

	drawCursor();

	glutSwapBuffers();
}

void mouseClickHandler(int button, int state, int x, int y) {
	gMousePosition.x = x;
	gMousePosition.y = y;
	gMouseDown = (state == GLUT_DOWN);

	glutPostRedisplay();
}

void mouseMoveHandler(int x, int y) {
	gMousePosition.x = x;
	gMousePosition.y = y;

	glutPostRedisplay();
}

void reshapeHandler(int width, int height) {
	setViewport(width, height);

	glutPostRedisplay();
}

void idleHandler(void) {
	gSpin = gSpin + 2.f;

	if (gSpin > 360.f) {
		gSpin = 0.f;
	}

	glutPostRedisplay();
}

void setViewport(int width, int height) {
	glViewport(0, 0, width, height);
}

void keyDownHandler(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}

	glutPostRedisplay();
}

void keyUpHandler(unsigned char key, int x, int y) {
	glutPostRedisplay();
}

void mouseEntryHandler(int state) {
	if (state == GLUT_LEFT) {
		CGDisplayShowCursor(CGMainDisplayID());
	}

	if (state == GLUT_ENTERED) {
		CGDisplayHideCursor(CGMainDisplayID());
	}
}

void drawCursor(void) {
	glBegin(GL_POLYGON);

	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex2i(gMousePosition.x, gMousePosition.y);
	glVertex2i(gMousePosition.x + 11, gMousePosition.y + 11);
	glVertex2i(gMousePosition.x + 11, gMousePosition.y + 12);
	glVertex2i(gMousePosition.x + 7, gMousePosition.y + 12);
	glVertex2i(gMousePosition.x + 9, gMousePosition.y + 17);
	glVertex2i(gMousePosition.x + 8, gMousePosition.y + 18);
	glVertex2i(gMousePosition.x + 7, gMousePosition.y + 18);
	glVertex2i(gMousePosition.x + 6, gMousePosition.y + 17);
	glVertex2i(gMousePosition.x + 5, gMousePosition.y + 14);
	glVertex2i(gMousePosition.x + 4, gMousePosition.y + 13);
	glVertex2i(gMousePosition.x + 1, gMousePosition.y + 16);
	glVertex2i(gMousePosition.x, gMousePosition.y + 16);
	
	glEnd();

  glBegin(GL_LINE_LOOP);

	glColor3f(1.f, 1.f, 1.f);
	glVertex2i(gMousePosition.x, gMousePosition.y);
	glVertex2i(gMousePosition.x + 11, gMousePosition.y + 11);
	glVertex2i(gMousePosition.x + 11, gMousePosition.y + 12);
	glVertex2i(gMousePosition.x + 7, gMousePosition.y + 12);
	glVertex2i(gMousePosition.x + 9, gMousePosition.y + 17);
	glVertex2i(gMousePosition.x + 8, gMousePosition.y + 18);
	glVertex2i(gMousePosition.x + 7, gMousePosition.y + 18);
	glVertex2i(gMousePosition.x + 6, gMousePosition.y + 17);
	glVertex2i(gMousePosition.x + 5, gMousePosition.y + 14);
	glVertex2i(gMousePosition.x + 4, gMousePosition.y + 13);
	glVertex2i(gMousePosition.x + 1, gMousePosition.y + 16);
	glVertex2i(gMousePosition.x, gMousePosition.y + 16);

	glEnd();
}

void setOrthographic(int width, int height) {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375f, 0.375f, 0.f);
}

void setPerspective(int width, int height) {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
