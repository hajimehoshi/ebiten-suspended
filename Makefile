PROG      := ebiten
PROG_TEST := ebiten_test
CXX := clang++

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-Iinclude \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc

LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

SRC := $(shell find src -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
GTEST_DIR := thrid_party/gtest

all: $(PROG).app
	open $<

test: $(PROG_TEST)
	./$<

$(PROG).app: $(PROG)
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp test.png $@/Contents/Resources

$(PROG): $(SRC)
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-o $@ \
		-O2 \
		main_samples.cpp

$(PROG_TEST): $(SRC) libgtest_main.a
	$(CXX) \
		$(CXXFLAGS) \
		-DGTEST_HAS_TR1_TUPLE=0 \
		$(LDFLAGS) \
		-I$(GTEST_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-lpthread \
		-L. -lgtest_main \
		main_test.cpp

libgtest_main.a:
	make -f Makefile.gtest $@

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG_TEST)
	rm -rf $(PROG).app
	rm -rf $(PROG_TEST).app
	rm -rf *.dSYM
	find . -name "*.a" -or -name "*.o" | xargs rm -f
