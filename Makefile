CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -framework OpenGL -framework GLUT -framework CoreGraphics

TARGET = bin/tui
SOURCES = src/main.c src/tui.c src/glad.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
