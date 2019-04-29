# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

# Modify this in order to customize the location for installation.
PREFIX = /usr/local

.PHONY: help doc test install clean

help:
	@echo "PREFIX (now '$(PREFIX)') in Makefile sets install directory."
	@echo "CXX in test/Makefile sets C++ compiler."
	@echo "Remember to use '-std=c++14' in your Makefile after install."
	@echo ""
	@echo "TARGETS"
	@echo "-------"
	@echo "all       Print this message."
	@echo "docs      Invoke doxygen to build documentation."
	@echo "test      Build and run unit tests."
	@echo "install   Copy headers to '$(PREFIX)/include'."
	@echo "clean     Remove objects and executable from test directory."

units.hpp: units.hpp.erb units.yml
	./process-template units.hpp.erb units.yml

doc: units.hpp
	@doxygen

test: units.hpp
	@$(MAKE) -C test

install: units.hpp
	@mkdir -p $(PREFIX)/include
	@cp -av vnix $(PREFIX)/include

clean:
	@$(MAKE) -C test clean
	@rm -rfv html
