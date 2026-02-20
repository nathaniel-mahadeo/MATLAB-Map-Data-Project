# Makefile for lab 11

project: project.c
        gcc project.c  gfx.o -lX11 -lm -o project -no-pie

clean:
        rm project
