PROG:=ebiten
CC:=gcc
CXX:=g++

CFLAGS:= \
	-W -Wall -Wextra \
	-fPIC \
	-L/opt/local/lib \
	-I/opt/local/include -Isrc

CXXFLAGS:= \
	$(CFLAGS) \
	-x objective-c++

LDFLAGS:= \
	-L/opt/local/lib \
	-pthread \
	-framework Cocoa -framework OpenGL \
	-lpng # TODO: link them statically

SRC:=$(shell find src -name "*.hpp" -or -name "*.cpp" -or -name "*.m")

all: $(PROG)_test
	./$(PROG)_test --gtest_color=yes

$(PROG): $(SRC)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-o $@ \
		-O2 \
		-DNDEBUG \
		src/main.cpp

$(PROG)_test: $(SRC)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-lgtest \
		src/main.cpp

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -rf *.dSYM
	find . -name "*.o" | xargs rm -f
