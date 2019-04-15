# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

# Modify this in order to customize the location for installation.
PREFIX=/usr/local

.PHONY: test install clean

test:
	$(MAKE) -C test

install:
	mkdir -p $(PREFIX)/include
	cp -a include/vnix $(PREFIX)/include

clean:
	$(MAKE) -C test clean
