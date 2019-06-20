#!/bin/bash
SRCS=srcs/*
BINDIR=bin
OBJDIR=objs
for f in $SRCS
do
	name=$(basename "$f" ".c")
	  gcc -m32 -o "$BINDIR/${name}_32" $f
	  gcc -m64 -o "$BINDIR/${name}_64" $f
	  gcc -m32 -c -o "$OBJDIR/${name}_32.o" $f
	  gcc -m64 -c -o "$OBJDIR/${name}_64.o" $f
	    # take action on each file. $f store current file name
	  done
