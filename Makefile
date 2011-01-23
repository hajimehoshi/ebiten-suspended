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
SRC_COCOA=$(shell find src -name "*.mm")
SRC_COCOA_HPP=$(SRC_COCOA:.mm=.hpp)
OBJ_COCOA=$(SRC_COCOA:.mm=.o)

all: $(PROG)_test
	./$(PROG)_test --gtest_color=yes

$(PROG): $(SRC) $(OBJ_COCOA)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-o $@ \
		-O2 \
		-DNDEBUG \
		src/main.cpp $(OBJ_COCOA)

$(PROG)_test: $(SRC) $(OBJ_COCOA)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-lgtest \
		-DEBITEN_TEST \
		src/main.cpp $(OBJ_COCOA)

$(OBJ_COCOA): %.o: %.mm %.hpp
	$(CC) \
		$(CFLAGS) \
		-c \
		-o $@ \
		$<

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	find . -name "*.o" | xargs rm -f
