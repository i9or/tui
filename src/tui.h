#ifndef TUI_H
#define TUI_H

#include <stdbool.h>

typedef struct {
	int x;
	int y;
} Vec2i;

typedef struct {
	bool isHot;
	bool isActive;
} UIState;

void tuiInit(void);
void tuiBegin(void);
void tuiEnd(void);

bool tuiButton(const char* label, int x, int y, int width, int height);

extern UIState gUIState;
extern Vec2i gMousePosition;
extern bool gMouseDown;

#endif /* TUI_H */
