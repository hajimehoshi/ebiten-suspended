PROG := ebiten
CXX := clang++

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-Isrc \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc

LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

SRC := $(shell find src -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

GTEST_DIR     := gtest
GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS    := $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h 

all: $(PROG).app
	open $<

test: $(PROG)_test
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
		src/main.cpp

$(PROG)_test: $(SRC) $(GTEST_HEADERS) libgtest_main.a
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
		src/main_test.cpp

libgtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

gtest-all.o: $(GTEST_SRCS)
	$(CXX) \
		-std=c++0x -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-W -Wall -Wextra -g \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include \
		-c \
		$(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS)
	$(CXX) \
		-std=c++0x -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-W -Wall -Wextra -g \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include \
		-c \
		$(GTEST_DIR)/src/gtest_main.cc

.PHONY: clean
clean:
	rm -f $(PROG)
	rm -f $(PROG)_test
	rm -rf $(PROG).app
	rm -rf $(PROG)_test.app
	rm -rf *.dSYM
	find . -name "*.a" -or -name "*.o" | xargs rm -f
