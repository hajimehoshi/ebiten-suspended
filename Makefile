PROG_SAMPLES := ebiten
PROG_TEST    := ebiten_test
CXX := clang++

CXXFLAGS := \
	-v \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 -pedantic \
	-fPIC \
	-Iinclude \
	-x objective-c++ -std=c++0x -stdlib=libc++ \
	-fobjc-arc \
	-DEBITEN_VERSION_COMMIT_UNIX_TIME="`git log --pretty="%ct" -1`" \
	-DEBITEN_VERSION_COMMIT_HASH="\"`git log --pretty="%H" -1`\"" \
	-DEBITEN_VERSION_COMMIT_MODIFIED="`git status --porcelain -uno | wc -l`"

LDFLAGS := \
	-framework Cocoa -framework OpenGL -framework QuartzCore

GTEST_DIR := third_party/gtest-1.6.0
V8_DIR    := third_party/v8-3.8.9

SRC_EBITEN  := $(shell find include -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_SAMPLES := $(shell find samples -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_TEST    := $(shell find test    -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

.PHONY: samples test clean

samples: $(PROG_SAMPLES).app
	open $<

# TODO: add xcodebuild?
test: bin/$(PROG_TEST)
	./$<

$(PROG_SAMPLES).app: bin/$(PROG_SAMPLES) samples/resources/*
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp samples/resources/* $@/Contents/Resources

bin/$(PROG_SAMPLES): $(SRC_EBITEN) $(SRC_SAMPLES) lib/libv8.a
	[ -f lib/libv8.a ]
	$(CXX) \
		$(CXXFLAGS) \
		$(LDFLAGS) \
		-I$(V8_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-Llib -lv8 \
		samples/main.cpp

bin/$(PROG_TEST): $(SRC_EBITEN) $(SRC_TEST) lib/libgtest_main.a lib/libv8.a
	[ -f lib/libgtest_main.a ]
	[ -f lib/libv8.a ]
	$(CXX) \
		$(CXXFLAGS) \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-Wno-variadic-macros \
		$(LDFLAGS) \
		-I$(GTEST_DIR)/include \
		-I$(V8_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-lpthread \
		-Llib -lgtest_main -lv8 \
		test/main.cpp

lib/libv8.a:
	(cd $(V8_DIR); \
		scons arch=x64)
	cp $(V8_DIR)/libv8.a $@

lib/libgtest_main.a:
	(cd $(GTEST_DIR)/src; \
		$(CXX) \
			-W -Wall -Wextra -stdlib=libc++ \
			-DGTEST_HAS_TR1_TUPLE=0 \
			-I.. -I../include \
			-g \
			-c \
			gtest-all.cc gtest_main.cc)
	$(AR) $(ARFLAGS) $@ $(GTEST_DIR)/src/*.o

clean:
	$(RM) -rf bin/*
	$(RM) -rf $(PROG_SAMPLES).app
	$(RM) -rf *.dSYM
	find . -name "*.o" | xargs $(RM) -rf
