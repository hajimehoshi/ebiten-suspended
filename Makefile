PROG:=ebiten
CXX:=clang++

CXXFLAGS:= \
	-W -Wall -Wextra \
	-fPIC \
	-I/usr/X11/include -Isrc \
	-x objective-c++

LDFLAGS:= \
	-L/usr/X11/lib \
	-pthread \
	-framework Cocoa -framework OpenGL \
	-lpng # TODO: link them statically

SRC:=$(shell find src -name "*.hpp" -or -name "*.cpp" -or -name "*.m")

all: $(PROG).app
	open $<

test: $(PROG)_test
	./$<

$(PROG).app: $(PROG)
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
		-lboost_unit_test_framework \
		-g \
		-o $@ \
		src/main_test.cpp

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -rf $(PROG).app
	rm -rf $(PROG)_test.app
	rm -rf *.dSYM
	find . -name "*.o" | xargs rm -f
