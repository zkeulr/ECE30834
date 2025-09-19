CXX = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = -lfltk -lfltk_gl -lGL -lGLU -ltiff -lm

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = a3

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dbg:
	rm -f $(OBJECTS) $(TARGET)
	$(MAKE) $(TARGET) CXXFLAGS="$(CXXFLAGS) -g -O0"

git: clean
	git add .
	git commit --allow-empty-message -m ""
	git push

recording:
	ffmpeg -framerate 30 -i frames/frame_%04d.tif -c:v libx264 -pix_fmt yuv420p camera_path.mp4

zip: clean
	zip -r $(TARGET).zip .

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).zip frames/*
	clear
