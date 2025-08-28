CXX = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = -lfltk -lfltk_gl -lGL -lGLU -ltiff -lm

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = a1

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

git: 
	git add .
	git commit --allow-empty-message -m ""

clean:
	rm -f $(OBJECTS) $(TARGET)