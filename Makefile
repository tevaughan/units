# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

# Modify this in order to customize the location for installation.
PREFIX = /usr/local

# Ruby-script used to generate files under vnix and under test.
export PROCESS_TEMPLATE = $(PWD)/erb-yml/process-template

.PHONY: all doc test install clean

all:
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

doc:
	@doxygen

test:
	@$(MAKE) -C test

install:
	@mkdir -p $(PREFIX)/include
	@cp -av vnix $(PREFIX)/include

clean:
	@$(MAKE) -C test clean
	@rm -rfv html
