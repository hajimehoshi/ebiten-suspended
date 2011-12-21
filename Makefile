PROG      := ebiten
PROG_TEST := ebiten_test
CXX := clang++

SAMPLES_DIR := samples
TEST_DIR    := test

all: $(PROG).app
	open $<

test: bin/$(PROG_TEST)
	./$<

$(PROG).app: bin/$(PROG)
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp test.png $@/Contents/Resources

bin/$(PROG):
	cd $(SAMPLES_DIR); make ../$@

bin/$(PROG_TEST):
	cd $(TEST_DIR); make ../$@

.PHONY: clean
clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf $(PROG).app
	rm -rf *.dSYM
