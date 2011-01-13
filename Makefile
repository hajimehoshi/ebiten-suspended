PROG=ebiten
CC=gcc
CXX=g++-mp-4.5

CFLAGS= \
	-W -Wall -Wextra \
	-g \
	-static-libgcc -fPIC \
	-L/opt/local/lib \
	-I/opt/local/include -Isrc \

CXXFLAGS= \
	$(CFLAGS) \
	-std=c++0x \
	-fno-deduce-init-list \
	-framework Cocoa -framework OpenGL -framework GLUT \
	-lboost_thread -lpng # TODO: link them statically

SRC=$(shell find src -name "*.hpp" -or -name "*.cpp")

all: $(PROG) $(PROG)_test
	./$(PROG)_test

$(PROG): $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		-o $@ \
		cocoa.o src/main.cpp

$(PROG)_test: $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		-o $@ \
		-lgtest \
		cocoa.o src/main_test.cpp

cocoa.o: src/ebiten/game/opengl/cocoa.m src/ebiten/game/opengl/cocoa.h
	$(CC) \
		$(CFLAGS) \
		-x objective-c \
		-c \
		-o $@ \
		$<

clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -f *.o
