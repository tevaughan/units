# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

# Modify this in order to customize the location for installation.
PREFIX=/usr/local

.PHONY: all doc test install clean

all:
	@echo "PREFIX (now '$(PREFIX)') in Makefile sets install directory."
	@echo "CXX in test/Makefile sets C++ compiler."
	@echo "Remember to use '-std=c++17' in your Makefile after install."
	@echo ""
	@echo "TARGETS"
	@echo "-------"
	@echo "all       Print this message."
	@echo "doc       Invoke doxygen to build documentation."
	@echo "test      Build and run unit tests."
	@echo "install   Copy headers to '$(PREFIX)/include'."
	@echo "clean     Remove objects and executable from test directory."

doc:
	@doxygen

test:
	@$(MAKE) -C test

install:
	@mkdir -p $(PREFIX)/include
	@cp -av include/vnix $(PREFIX)/include
	@rm -fv $(PREFIX)/include/vnix/units.md

clean:
	@$(MAKE) -C test clean
	@rm -rfv doc
