CXX := clang++

libgtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

gtest-all.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I. -Iinclude \
		-g \
		-c \
		src/gtest-all.cc

gtest_main.o:
	$(CXX) \
		-W -Wall -Wextra -stdlib=libc++ \
		-DGTEST_HAS_TR1_TUPLE=0 \
		-I. -Iinclude \
		-g \
		-c \
		src/gtest_main.cc
