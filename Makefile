# TODO: Check include guards

PROG_SHRIMP  := shrimp
PROG_SAMPLES := ebiten
PROG_TEST    := ebiten_test
CXX := clang++

CXXFLAGS := \
	-W -Wall -Wextra -Wmissing-prototypes -Wshorten-64-to-32 \
	-std=c++0x -stdlib=libc++ \
	-fPIC \
	-Iinclude \
	-DEBITEN_VERSION_COMMIT_UNIX_TIME="`git log --pretty="%ct" -1`" \
	-DEBITEN_VERSION_COMMIT_HASH="\"`git log --pretty="%H" -1`\"" \
	-DEBITEN_VERSION_COMMIT_MODIFIED="`git status --porcelain -uno | wc -l`"

LDFLAGS := \
	 -stdlib=libc++ -framework Cocoa -framework OpenGL -framework QuartzCore

GTEST_DIR    := third_party/gtest-1.6.0
SQUIRREL_DIR := third_party/squirrel-3.0.2

SRC_INCLUDE := $(shell find include -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_SAMPLES := $(shell find samples -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")
SRC_TEST    := $(shell find test    -name "*.hpp" -or -name "*.cpp" -or -name "*.mm")

.PHONY: shrimp samples test clean

shrimp: $(PROG_SHRIMP).app
	open $<

samples: $(PROG_SAMPLES).app
	open $<

# TODO: add xcodebuild?
test: bin/$(PROG_TEST)
	./$<

$(PROG_SHRIMP).app: bin/$(PROG_SHRIMP) samples/resources/*
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp samples/resources/* $@/Contents/Resources

$(PROG_SAMPLES).app: bin/$(PROG_SAMPLES) samples/resources/*
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp samples/resources/* $@/Contents/Resources

bin/$(PROG_SHRIMP): lib/$(PROG_SHRIMP).o
	$(CXX) \
		$(LDFLAGS) \
		-g \
		-o $@ \
		-O0 \
		`wx-config --libs --gl-libs` \
		$<

lib/$(PROG_SHRIMP).o: $(SRC_INCLUDE) $(SRC_SAMPLES)
	$(CXX) \
		$(CXXFLAGS) \
		-c \
		-g \
		-o $@ \
		-O0 \
		`wx-config --cppflags` \
		samples/main_shrimp.cpp

bin/$(PROG_SAMPLES): $(SRC_INCLUDE) $(SRC_SAMPLES) lib/libsquirrel.a lib/libsqstdlib.a
	$(CXX) \
		$(CXXFLAGS) \
		-x objective-c++ \
		-fobjc-arc \
		$(LDFLAGS) \
		-I$(SQUIRREL_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-Llib -lsquirrel -lsqstdlib \
		samples/main.cpp

bin/$(PROG_TEST): $(SRC_INCLUDE) $(SRC_TEST) lib/libgtest_main.a lib/libsquirrel.a lib/libsqstdlib.a
	$(CXX) \
		$(CXXFLAGS) \
		-x objective-c++ \
		-fobjc-arc \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-Wno-variadic-macros \
		$(LDFLAGS) \
		-I$(GTEST_DIR)/include \
		-I$(SQUIRREL_DIR)/include \
		-g \
		-o $@ \
		-O0 \
		-lpthread \
		-Llib -lgtest_main -lsquirrel -lsqstdlib \
		test/main.cpp

lib/libsquirrel.a:
	(cd $(SQUIRREL_DIR)/squirrel; \
		$(CXX) \
			-W -Wall -Wno-missing-field-initializers -Wno-unused-parameter \
			-stdlib=libc++ \
			-m64 -D_SQ64 \
			-I../include \
			-O2 \
			-c \
			*.cpp)
	$(AR) $(ARFLAGS) $@ $(SQUIRREL_DIR)/squirrel/*.o

lib/libsqstdlib.a:
	(cd $(SQUIRREL_DIR)/sqstdlib; \
		$(CXX) \
			-W -Wall -Wno-missing-field-initializers -Wno-unused-parameter \
			-stdlib=libc++ \
			-m64 -D_SQ64 \
			-I../include \
			-O2 \
			-c \
			*.cpp)
	$(AR) $(ARFLAGS) $@ $(SQUIRREL_DIR)/sqstdlib/*.o

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
