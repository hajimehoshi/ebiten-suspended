PROG:=ebiten
CC:=gcc
CXX:=g++-mp-4.5

CFLAGS:= \
	-W -Wall -Wextra \
	-fPIC \
	-L/opt/local/lib \
	-I/opt/local/include -Isrc

CXXFLAGS:= \
	$(CFLAGS) \
	-std=c++0x \
	-fno-deduce-init-list \

LDFLAGS:= \
	-pthread \
	-static-libgcc \
	-framework Cocoa -framework OpenGL -framework GLUT \
	-lpng # TODO: link them statically

SRC:=$(shell find src -name "*.hpp" -or -name "*.cpp")
# SRC_MM=$(shell find src -name "*.mm")

all: $(PROG)_test
	./$(PROG)_test --gtest_color=yes

$(PROG): $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-o $@ \
		-O2 \
		-DNDEBUG \
		src/main.cpp cocoa.o

$(PROG)_test: $(SRC) cocoa.o
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-lgtest \
		-DEBITEN_TEST \
		src/main.cpp cocoa.o

cocoa.o: src/ebiten/game/graphics/opengl/cocoa.mm src/ebiten/game/graphics/opengl/cocoa.hpp
	$(CC) \
		$(CFLAGS) \
		-c \
		-o $@ \
		$<

clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -f *.o
