PROG=ebiten
CC=gcc
CXX=g++-mp-4.5

CFLAGS= \
	-W -Wall -Wextra \
	-static-libgcc -fPIC \
	-L/opt/local/lib \
	-I/opt/local/include -Isrc

CXXFLAGS= \
	$(CFLAGS) \
	-std=c++0x -pthread \
	-fno-deduce-init-list \
	-framework Cocoa -framework OpenGL -framework GLUT \
	-lpng # TODO: link them statically

SRC=$(shell find src -name "*.hpp" -or -name "*.cpp")

all: $(PROG)_test
	./$(PROG)_test --gtest_color=yes

$(PROG): $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		-o $@ \
		-O2 \
		-DNDEBUG \
		cocoa.o src/main.cpp

$(PROG)_test: $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		-g \
		-o $@ \
		-lgtest \
		-DEBITEN_TEST \
		cocoa.o src/main.cpp

cocoa.o: src/ebiten/game/opengl/cocoa.mm src/ebiten/game/opengl/cocoa.hpp
	$(CC) \
		$(CFLAGS) \
		-c \
		-o $@ \
		$<

clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -f *.o
