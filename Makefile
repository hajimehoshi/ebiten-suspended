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

all: $(PROG)_test.app
	open $(PROG)_test.app --args --gtest_color=yes

$(PROG).app: $(PROG)
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS/

$(PROG)_test.app: $(PROG)_test
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS/

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
	rm -rf $(PROG).app
	rm -rf $(PROG)_test.app
	rm -rf *.dSYM
	find . -name "*.o" | xargs rm -f
