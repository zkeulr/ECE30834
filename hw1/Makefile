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
	git push

capture:
	ffmpeg -f x11grab -framerate 30 -t 10.5 -s 1920x1080 -i :0.0 -c:v libx264 -preset medium -crf 23 output.mp4 ./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	clear