# Makefile for building "cre" from main.cpp

CXX := g++
CXXFLAGS := -Wall -Wextra -g 
TARGET := cre
SRCS := main.cpp \
		 GameBoard.cpp \
		 GameSettings.cpp \
		 GameCell.cpp \
		 GameCursor.cpp \
		 GamePlayer.cpp \
		 DisplayHandler.cpp \
		 ExplosionHandler.cpp \
		 ExplosionQueue.cpp \
		 GameFrameQueue.cpp \

BUILD_DIR := build
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
#rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean