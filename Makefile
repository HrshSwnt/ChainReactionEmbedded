# Makefile for building "cre" from src/*.cpp

CXX := g++
CXXFLAGS := -Wall -Wextra -g 
TARGET := cre
SRCS := src/main.cpp \
		 src/GameBoard.cpp \
		 src/GameSettings.cpp \
		 src/GameCell.cpp \
		 src/GameCursor.cpp \
		 src/GamePlayer.cpp \
		 src/DisplayHandler.cpp \
		 src/ExplosionHandler.cpp \
		 src/ExplosionQueue.cpp \
		 src/GameFrameQueue.cpp \

SRC_DIR := src
BUILD_DIR := build
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
