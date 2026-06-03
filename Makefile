CXX = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude

SRC_DIR = src
INC_DIR = include

SOURCES = $(SRC_DIR)/Profile.cpp $(SRC_DIR)/User.cpp $(SRC_DIR)/Post.cpp $(SRC_DIR)/Comment.cpp $(SRC_DIR)/SocialNetwork.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = SocialNetwork

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@