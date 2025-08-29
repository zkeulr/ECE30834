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

record: $(TARGET)
	rm -f name.mp4
	./$(TARGET) &
	wf-recorder -g "$$(swaymsg -t get_tree \
	  | jq -r '.. | select(.name? == "Name")? \
	  | .rect | "\(.x),\(.y) \(.width)x\(.height)"')" -f name.mp4

windows: 
	x86_64-w64-mingw32-g++ -std=c++11 -Wall \
		src/*.cpp -o a1.exe \
		-I/path/to/fltk/include \
		-L/path/to/fltk/lib \
		-lfltk -lfltk_gl -lopengl32 -lglu32 -ltiff

clean:
	rm -f $(OBJECTS) $(TARGET)
	clear