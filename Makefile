PROG_SAMPLES := ebiten
PROG_TEST    := ebiten_test
CXX := clang++

SAMPLES_DIR := samples
TEST_DIR    := test

all: $(PROG).app
	open $<

test: bin/$(PROG_TEST)
	./$<

$(PROG_SAMPLES).app: $(PROG_SAMPLES)
	mkdir -p $@/Contents/MacOS
	cp bin/$< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp test.png $@/Contents/Resources

$(PROG_SAMPLES):
	cd $(SAMPLES_DIR); make $@

$(PROG_TEST):
	cd $(TEST_DIR); make $@

.PHONY: clean
clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf $(PROG).app
	rm -rf *.dSYM
