# SRCS contains a list of every cpp-file for which an object-file will be
# compiled.  SRCS is used explicitly by the autodependency code.
SRCS = tests.cpp\
 gcd-test.cpp\
 bit-range-test.cpp\
 rational-base-test.cpp\
 normalized-pair-test.cpp\
 encoding-test.cpp

# These variables are used explicitly by the autodependency code.
CPPFLAGS = -I../include
CXXFLAGS = -g -O0 -std=c++17 -Wall
CXX      = g++

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

tests: $(SRCS:.cpp=.o)

clean:
	@rm -frv $(DEPDIR)
	@rm -fv *.o
	@rm -fv tests

# This should be the last line.
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
