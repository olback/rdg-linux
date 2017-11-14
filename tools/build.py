#!/usr/bin/python

import os.path

os.system('glib-compile-resources --target=src/resources.c --generate-source src/gresource.xml')
os.system('gcc src/main.c -o rdg.app -Wall -rdynamic `pkg-config --cflags --libs gtk+-3.0`')
os.remove('src/resources.c')
