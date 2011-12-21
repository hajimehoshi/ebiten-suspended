CXX := clang++

EBITEN_ROOT_DIR := ../..
OBJ := $(EBITEN_ROOT_DIR)/lib/gtest-all.o $(EBITEN_ROOT_DIR)/lib/gtest_main.o

$(EBITEN_ROOT_DIR)/lib/libgtest_main.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(EBITEN_ROOT_DIR)/lib/gtest-all.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I. -Iinclude \
		-g \
		-o $@ \
		-c \
		src/gtest-all.cc

$(EBITEN_ROOT_DIR)/lib/gtest_main.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I. -Iinclude \
		-g \
		-o $@ \
		-c \
		src/gtest_main.cc
