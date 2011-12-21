PROG := ebiten
CXX := clang++

EBITEN_ROOT_DIR := ..

SRC := $(shell find $(EBITEN_ROOT_DIR)/include -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-I$(EBITEN_ROOT_DIR)/include \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc

LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

$(EBITEN_ROOT_DIR)/bin/$(PROG): $(SRC)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-o $@ \
		-O2 \
		main.cpp
