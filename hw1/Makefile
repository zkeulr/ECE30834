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
	sleep 0.5
	timeout 10.5s recordmydesktop --width=640 --height=480 --fps=30 --delay=0s --no-sound --overwrite -o name.ogv
	ffmpeg -i name.ogv -c:v libx264 -preset medium -crf 23 name.mp4
	rm name.ogv
	pkill -f "./$(TARGET)" || true
	
clean:
	rm -f $(OBJECTS) $(TARGET)
	clear