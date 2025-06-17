# Makefile for building "cre" from main.cpp

CXX := g++
CXXFLAGS := -Wall -Wextra
TARGET := cre
SRCS := main.cpp \
		 GameBoard.cpp \
		 GameSettings.cpp \
		 GameCell.cpp \
		 GameCursor.cpp

OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean