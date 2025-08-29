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

capture: $(TARGET)
	./$(TARGET) &
	sleep 1
	ffmpeg -f x11grab -framerate 30 -t 10.5 -s 640x480 -i $$DISPLAY -c:v libx264 -preset medium -crf 23 name.mp4
	pkill -f "./$(TARGET)" || true

clean:
	rm -f $(OBJECTS) $(TARGET)
	clear