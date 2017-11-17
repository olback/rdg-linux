CC = gcc
CFLAGS = -Wall -rdynamic `pkg-config --cflags --libs gtk+-3.0`
OUT_FILE = rdg
SOURCE = src/main.c
RESOURCE_TARGET = src/resources.c
RESOURCE_SOURCE = src/gresource.xml

build:
	glib-compile-resources --target=$(RESOURCE_TARGET) --generate-source $(RESOURCE_SOURCE)
	$(CC) -o $(OUT_FILE) $(SOURCE) $(CFLAGS)
	make clean

run:
	make build
	./$(OUT_FILE)

clean:
	rm src/resources.c
