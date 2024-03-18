TARGET = viewer
# C++ Source Code Files
CXXFILES = $(TARGET).cc callbacks.cc global.cc trackball.cc util.cc
# C++ Headers Files
HEADERS = callbacks.h drawobject.h global.h stb_image.h timerutil.h trackball.h util.h 

DO_UNITTESTS = "False"

CXX = clang++
CXXFLAGS += -g -O3 -Wall -pedantic -pipe -std=c++17
LDFLAGS += -g -O3 -Wall -pedantic -pipe -std=c++17

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS += -D LINUX -nostdinc++ -I/usr/include/c++/11 -I/usr/include/x86_64-linux-gnu/c++/11
	LDFLAGS += -L /usr/lib/gcc/x86_64-linux-gnu/11 -lGL -lGLU -lglfw -lGLEW -ltinyobjloader
	SED = sed
	GTESTINCLUDE = -D LINUX -nostdinc++ -I/usr/include/c++/11 -I/usr/include/x86_64-linux-gnu/c++/11
	GTESTLIBS = -L /usr/lib/gcc/x86_64-linux-gnu/11 -lgtest -lgtest_main -lpthread
	# Note in Ubuntu 22 clang++ has it's own std. library
	# clang++ -nostdinc++ -nostdlib++ -isystem /usr/lib/llvm-14/include/c++/v1 -L /usr/lib/llvm-14/lib -Wl,-rpath,/usr/lib/llvm-14/lib -lc++ -std=c++17
endif
ifeq ($(UNAME_S),Darwin)
	ifeq (,$(wildcard "/opt/local/bin/port"))
	# Use MacPorts clang++'s standard library
		CXXFLAGS += -D OSX -D GL_SILENCE_DEPRECATION -nostdinc++ -I/opt/local/include/libcxx/v1 -I/opt/local/include
		LDFLAGS += -L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -L/opt/local/lib/libcxx -L/opt/local/lib
		LLDLIBS += -lglfw -framework OpenGL -lGLEW -ltinyobjloader
		SED = gsed
		GTESTINCLUDE = -I/opt/local/include -I/opt/local/src/googletest
		GTESTLIBS = -L/opt/local/lib -lgtest -lgtest_main
	else
		# Use Apple's standard library (not recommended)
		CXXFLAGS += -D OSX
	endif
endif
UNAME_M = $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
	CXXFLAGS += -D AMD64
endif
ifneq ($(filter %86,$(UNAME_M)),)
	CXXFLAGS += -D IA32
endif
ifneq ($(filter arm%,$(UNAME_M)),)
	CXXFLAGS += -D ARM
endif

GTEST_OUTPUT_FORMAT ?= "json"
GTEST_OUTPUT_FILE ?= "test_detail.json"

DOXYGEN = doxygen
DOCDIR = doc

MAKEHEADERS := $(shell command -v makeheaders 2>/dev/null)

OBJECTS = $(CXXFILES:.cc=.o)

DEP = $(CXXFILES:.cc=.d)

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PART_PATH := $(dir $(MKFILE_PATH))
LAB_PART := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))

.SILENT: doc lint format authors test

default all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LLDLIBS)

-include $(DEP)

%.d: %.cc
	set -e; $(CXX) -MM $(CXXFLAGS) $< 	| sed 's/\($*\)\.o[ :]*/.o $@ : /g' > $@; 	[ -s $@ ] || rm -f $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

clean:
	-rm -f $(OBJECTS) core $(TARGET).core

spotless: clean cleanunittest
	-rm -f $(TARGET) $(DEP) a.out
	-rm -rf $(DOCDIR)
	-rm -rf $(TARGET).dSYM
	-rm -f compile_commands.json

doc: $(CXXFILES) $(HEADERS)
	(cat Doxyfile; echo "PROJECT_NAME = $(TARGET)") | $(DOXYGEN) -

compilecmd:
	@echo "$(CXX) $(CXXFLAGS)"

format:
	@python3 .action/checks.py format $(LAB_PART)

lint:
	@python3 .action/checks.py lint $(LAB_PART)

authors:
	@python3 .action/checks.py authors $(LAB_PART)

test:
	@python3 .config/system_test.py $(LAB_PART) $(TARGET)

ifneq ($(DO_UNITTESTS), "True")
unittest:
	@echo "No unit tests for $(LAB_PART)"
else
unittest: cleanunittest utest

utest: $(TARGET)_functions.o $(TARGET)_unittest.cc
	@$(CXX) $(GTESTINCLUDE) $(LDFLAGS) -o unittest $(TARGET)_unittest.cc $(TARGET)_functions.o $(GTESTLIBS)
	@./unittest --gtest_output=$(GTEST_OUTPUT_FORMAT):$(GTEST_OUTPUT_FILE)

endif

cleanunittest:
		-@rm -rf unittest.dSYM > /dev/null 2>&1 || true
		-@rm unittest test_detail.json > /dev/null 2>&1 || true
