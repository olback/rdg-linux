#
#	rdg-linux © olback 2018
#

CC = gcc
# CFLAGS = -Wall -rdynamic -lpthread `pkg-config --cflags --libs libcjson libcurl libnotify gtk+-3.0`
CFLAGS = -Wall -rdynamic -lpthread `pkg-config --cflags --libs libcurl libnotify gtk+-3.0`
OUT_FILE = rdg.out
SOURCE = src/main.c
RESOURCE_TARGET = src/include/resources.c
RESOURCE_SOURCE = src/gresource.xml
INSTALL_PATH = /usr/bin/rdg

build:
	make resource
	$(CC) -o $(OUT_FILE) $(SOURCE) $(CFLAGS)
	sha512sum $(OUT_FILE) > sha512.sum
	make clean
	@echo "Done!"

run:
	make build
	./$(OUT_FILE) -t

resource:
	glib-compile-resources --target=$(RESOURCE_TARGET) --generate-source $(RESOURCE_SOURCE)

clean:
	rm $(RESOURCE_TARGET)

install:
	sudo cp $(OUT_FILE) $(INSTALL_PATH)
	@echo "Done installing"

uninstall:
	sudo rm $(INSTALL_PATH)
	@echo "Done uninstalling"

update:
	git pull
	make build
	make uninstall
	make install
	@echo "Done updating!"
