CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
TARGET = bin/app.exe

SRC = main.c \
      $(wildcard src/*.c) \
      $(wildcard src/*/*.c) \
      $(wildcard src/*/*/*.c) \
      $(wildcard src/*/*/*/*.c)

.PHONY: build run create_dirs clean deepClean

build: create_dirs $(TARGET)

create_dirs:
	@mkdir -p bin
	@mkdir -p data/backups
	@mkdir -p data/exports

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "✅ Compilation Successful!"

run: build
	@echo "🚀 Launching Application..."
	./$(TARGET)

clean:
	@rm -f $(TARGET)
	@echo "🧹 Cleaned up executable binary!"

deepClean:
	@rm -rf bin/app.exe
	@rm -rf data/*
	@echo "🧹 Cleaned up the project completely!"
