# Copyright 2019, Thomas E. Vaughan; all rights reserved.
# Distributed under the terms of the BSD three-clause license; see LICENSE.

# Modify this as necessary so that unit-test of compatibility with eigen will
# be compiled and run.
EIGEN_DIR = /usr/include/eigen3/Eigen

EIGEN_COMPAT_TEST =\
 $(shell if test -r $(EIGEN_DIR); then echo eigen-compat-test.cpp; fi)

# SRCS contains a list of every cpp-file for which an object-file will be
# compiled.  SRCS is used explicitly by the autodependency code.
SRCS = tests.cpp\
 bit-range-test.cpp\
 common-denom-test.cpp\
 dim-test.cpp\
 dimval-test.cpp\
 dyndim-base-test.cpp\
 encoding-test.cpp\
 gcd-test.cpp\
 normalized-pair-test.cpp\
 rational-test.cpp\
 statdim-base-test.cpp\
 $(EIGEN_COMPAT_TEST)

# These variables are used explicitly by the autodependency code.
CPPFLAGS = -I.. #-isystem /usr/include/clang/7/include
CXXFLAGS = -g -O0 -std=c++14 -Wall
CXX      = clang++

# CC is used implicitly by the linker and must be set equal to the value stored
# in CXX.  C++ objects require special handling, and so the C++ compiler must
# be invoked as the linker.  The C compiler will not do.
CC = $(CXX)

# ---------- BEG Automatic dependencies for C and C++ files. ----------
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d
%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d
# ---------- END Automatic dependencies for C and C++ files. ----------

.PHONY: all clean

all: tests
	@rm -frv kcov
	@kcov --include-pattern=vnix kcov ./tests

tests: $(SRCS:.cpp=.o)

clean:
	@rm -frv kcov
	@rm -frv $(DEPDIR)
	@rm -fv *.o
	@rm -fv tests

# This should be the last line.
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
