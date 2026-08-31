DISC ?=

.PHONY: all setup deps check progress clean

all:
	./build.sh --resplit

setup:
	@test -n "$(DISC)" || { echo "usage: make setup DISC=/path/to/your/kaze.cue"; exit 1; }
	./configure.py "$(DISC)"
	./configure.py --deps

deps:
	./configure.py --deps

check: progress
	python3 tools/island_audit.py --board

progress:
	python3 tools/progress.py

clean:
	rm -rf build
