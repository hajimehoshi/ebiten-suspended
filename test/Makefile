PROG := ebiten_test
CXX := clang++

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-Iinclude \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc

LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

EBITEN_ROOT_DIR := ..
GTEST_DIR       := $(EBITEN_ROOT_DIR)/thrid_party/gtest-1.6.0

SRC := $(shell find $(EBITEN_ROOT_DIR)/include -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

$(EBITEN_ROOT_DIR)/bin/$(PROG): $(SRC) $(EBITEN_ROOT_DIR)/lib/libgtest_main.a
	$(CXX) \
		$(CXXFLAGS) \
		-DGTEST_HAS_TR1_TUPLE=0 \
		$(LDFLAGS) \
		-I$(EBITEN_ROOT_DIR)/include \
		-I$(GTEST_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-lpthread \
		-L$(EBITEN_ROOT_DIR)/lib -lgtest_main \
		main.cpp

$(EBITEN_ROOT_DIR)/lib/libgtest_main.a:
	cd $(GTEST_DIR); make ../$@
