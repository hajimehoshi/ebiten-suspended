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
	-L/opt/local/lib \
	-pthread \
	-static-libgcc \
	-framework Cocoa -framework OpenGL \
	-lpng # TODO: link them statically

SRC:=$(shell find src -name "*.hpp" -or -name "*.cpp")
SRC_COCOA=$(shell find src -name "*.mm")
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

$(PROG)_test.o: $(SRC) $(OBJ_COCOA)
	$(CXX) \
		$(CXXFLAGS) \
		-g \
		-c \
		-o $@ \
		-DEBITEN_TEST \
		src/main.cpp

$(PROG)_test: $(PROG)_test.o
	$(CXX) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-lgtest \
		$< $(OBJ_COCOA)

$(OBJ_COCOA): %.o: %.mm %.m %.hpp
	$(CC) \
		$(CFLAGS) \
		-c \
		-g \
		-o $@ \
		$<

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -rf *.dSYM
	find . -name "*.o" | xargs rm -f
