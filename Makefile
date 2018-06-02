CC = gcc
CFLAGS = -Wall -rdynamic `pkg-config --cflags --libs gtk+-3.0`
OUT_FILE = rdg.out
SOURCE = src/main.c
RESOURCE_TARGET = src/include/resources.c
RESOURCE_SOURCE = src/gresource.xml
INSTALL_PATH = /usr/bin/rdg

build:
	make resource
	$(CC) -o $(OUT_FILE) $(SOURCE) $(CFLAGS)
	sha512sum rdg > sha512.sum
	make clean

run:
	make build
	./$(OUT_FILE) -t

resource:
	glib-compile-resources --target=$(RESOURCE_TARGET) --generate-source $(RESOURCE_SOURCE)

clean:
	rm $(RESOURCE_TARGET)

install:
	cp $(OUT_FILE) $(INSTALL_PATH)

uninstall:
	rm $(INSTALL_PATH)
