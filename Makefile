PROG=ebiten
CXX=g++-mp-4.5
CXXFLAGS=-g -std=c++0x \
	-W -Wall -Wextra -fno-deduce-init-list \
	-L/opt/local/lib \
	-lboost_thread -lpng \
	-framework Cocoa -framework OpenGL -framework GLUT \
	-isystem/usr/local/include -Isrc

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
		-lboost_unit_test_framework \
		-DEBITEN_TEST \
		-o $@ \
		cocoa.o src/main_test.cpp

cocoa.o: src/ebiten/game/opengl/cocoa.m src/ebiten/game/opengl/cocoa.h
	gcc \
		-c -g -W -Wall -Wextra \
		-o $@ \
		-isystem/usr/local/include -Isrc \
		$<

clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -f *.o
