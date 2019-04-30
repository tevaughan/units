# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

# Modify this in order to customize the location for installation.
PREFIX = /usr/local

# Basename for each gnenerated hpp file.
GENERATED_CXX = dim-base-off units

.PHONY: help doc test install clean $(GENERATED_CXX)

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

% : %.erb units.yml
	./process-template $^

dim-base-off: dim-base-off.hpp
	@mv -v dim-base-off.hpp vnix/units

units: units.hpp
	@mv -v units.hpp vnix

doc: $(GENERATED_CXX)
	@doxygen

test: $(GENERATED_CXX)
	@$(MAKE) -C test

install: $(GENERATED_CXX)
	@mkdir -p $(PREFIX)/include
	@cp -av vnix $(PREFIX)/include

clean:
	@$(MAKE) -C test clean
	@rm -rfv html
	@rm -fv $(GENERATED_CXX:=.hpp)
	@rm -fv vnix/units/dim-base-off.hpp vnix/units.hpp
