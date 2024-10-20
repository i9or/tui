#include <string.h>
#include <stdio.h>

#include <glad/gl.h>
#include <GLUT/glut.h>

#include "tui.h"

UIState gUIState;
Vec2i gMousePosition;
bool gMouseDown;

void tuiInit(void) {
	gUIState.isHot = false;
	gUIState.isActive = false;
	gMouseDown = false;
}

void tuiBegin(void) {
	gUIState.isHot = false;
}

void tuiEnd(void) {
	if (!gMouseDown) {
		gUIState.isActive = false;
	}
}

static void drawRect(int x, int y, int width, int height, float color[4]) {
	glColor4fv(color);
	
	glBegin(GL_QUADS);
	
	glVertex2i(x, y);
	glVertex2i(x + width, y);
	glVertex2i(x + width, y + height);
	glVertex2i(x, y + height);

	glEnd();
}

static bool isInRect(int x, int y, int width, int height) {
	return gMousePosition.x >= x && gMousePosition.x <= x + width &&
		gMousePosition.y >= y && gMousePosition.y <= y + height;
}

static void drawText(const char* text, int x, int y) {
	// TODO: to be implemented
}

bool tuiButton(const char* label, int x, int y, int width, int height) {
	bool isClicked = false;

	if (isInRect(x, y, width, height)) {
		gUIState.isHot = true;
		
		if (gMouseDown && !gUIState.isActive) {
			gUIState.isActive = true;
		}
	}

	if (gUIState.isHot && gUIState.isActive && !gMouseDown) {
		isClicked= true;
	}

	float color[4] = { 0.6f, 0.6f, 0.6f, 1.f };

	if (gUIState.isHot) {
		color[0] = color[1] = color[2] = 0.8f;
	}

	if (gUIState.isActive) {
		color[0] = color[1] = color[2] = 0.4f;
	}

	drawRect(x, y, width, height, color);
	glColor3f(0.f, 0.f, 0.f);
	drawText(label, x + 5, y + 5);

	return isClicked;
}
