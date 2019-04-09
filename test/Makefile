CXX = clang++
CC = $(CXX)

CPPFLAGS = -I../include
CXXFLAGS = -g -O0 -std=c++17 -Wall
SRCS = $(shell ls *.cpp)

# ---------- BEG Automatic dependencies for C and C++ files. ----------
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
SRCS=$(shell echo *.cpp)
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

.PHONY: clean

units-test: units-test.o

clean:
	@rm -frv $(DEPDIR)
	@rm -fv *.o
	@rm -fv units-test

# This must be the last line.
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
