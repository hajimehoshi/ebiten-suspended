PROG_SAMPLES := ebiten
PROG_TEST    := ebiten_test
CXX := clang++

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-Iinclude \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc
LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

GTEST_DIR := thrid_party/gtest-1.6.0

SRC_EBITEN  := $(shell find include -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_SAMPLES := $(shell find samples -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_TEST    := $(shell find test    -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

.PHONY: samples test clean

samples: $(PROG_SAMPLES).app
	open $<

test: bin/$(PROG_TEST)
	./$<

$(PROG_SAMPLES).app: bin/$(PROG_SAMPLES)
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp test.png $@/Contents/Resources

bin/$(PROG_SAMPLES): $(SRC_EBITEN) $(SRC_SAMPLES)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-O0 \
		samples/main.cpp

bin/$(PROG_TEST): $(SRC_EBITEN) $(SRC_TEST) lib/libgtest_main.a
	$(CXX) \
		$(CXXFLAGS) \
		-DGTEST_HAS_TR1_TUPLE=0 \
		$(LDFLAGS) \
		-I$(GTEST_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-lpthread \
		-Llib -lgtest_main \
		test/main.cpp

lib/libgtest_main.a: lib/gtest-all.o lib/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

lib/gtest-all.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I$(GTEST_DIR) -I$(GTEST_DIR)/include \
		-g \
		-o $@ \
		-c \
		$(GTEST_DIR)/src/gtest-all.cc

lib/gtest_main.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I$(GTEST_DIR) -I$(GTEST_DIR)/include \
		-g \
		-o $@ \
		-c \
		$(GTEST_DIR)/src/gtest_main.cc

clean:
	$(RM) -rf bin/*
	$(RM) -rf lib/*
	$(RM) -rf $(PROG_SAMPLES).app
	$(RM) -rf *.dSYM
