#!/usr/bin/python

import os.path

os.system('glib-compile-resources --target=src/resources.c --generate-source src/gresource.xml')
os.system('gcc src/main.c -Wall -rdynamic `pkg-config --cflags --libs gtk+-3.0`')