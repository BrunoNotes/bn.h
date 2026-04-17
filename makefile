CONFIG ?= debug
# CONFIG ?= release

APP_NAME = main

CC := gcc
CFLAGS = -Wall -Wextra
# DEBUG_FLAGS = -DDEBUG -O0 -g -fsanitize=address
DEBUG_FLAGS = -DDEBUG -O0 -g
RELEASE_FLAGS = -DNDEBUG -O3

ifeq ($(CONFIG), debug)
	CFLAGS += $(DEBUG_FLAGS)
else
	CFLAGS += $(RELEASE_FLAGS)
endif

ifeq ($(OS),Windows_NT)
    EXEC = $(APP_NAME).exe
else
    EXEC = $(APP_NAME)
endif

all: build run

run:
	@./bin/$(EXEC)

build:
	@-mkdir ./bin
	$(CC) src/main.c $(CFLAGS) -o bin/$(EXEC)
