# Copyright 2019  Thomas E. Vaughan
# Distrubited under the terms of GPL, Version 3 or later.  See LICENSE.

PREFIX=/usr/local

.PHONY: test install generate

test: generate
	$(MAKE) -C test

install: generate
	cp -a include/vnix $(PREFIX)/include

generate:
	$(MAKE) -C include/vnix/units
