# Makefile for Zombie vs Humans Simulation - Assignment 4
# CSCI 460 - Operating Systems
# Author: Ross Moon

#Macros to specify compiler, compiler/linking flags, target file
CC  = gcc 
CFLAGS = -Wall -Werror -g -lpthread
LD = gcc
LDFLAGS = 
LIBS = 
OBJS = Driver.o mem.o
EXEC = Driver

#Rules to create target file Driver
# If any files on line with colon are modified, then recompile the object file
all:		$(EXEC)

$(EXEC):	$(OBJS)
		$(LD) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

mem.o: 	mem.c mem.h
		$(CC) $(CFLAGS) -c mem.c

Driver.o:	Driver.c
		$(CC) $(CFLAGS) -c Driver.c

clean:
		rm -f $(OBJS) $(EXEC) 

