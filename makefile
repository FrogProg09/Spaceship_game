CXXFLAGS=$(shell pkg-config --cflags sdl2) -Wall
LDFLAGS=$(shell pkg-config --libs sdl2) -Wall
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
main: main.o
	$(CXX) -o $@ $< $(LDFLAGS)
