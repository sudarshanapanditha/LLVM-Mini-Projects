LLVM_CONFIG?=llvm-config-8

ifndef VERBOSE
QUIET:=@
endif

SRC_DIR?=$(PWD)
CXX=clang++-8
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)
COMMON_FLAGS=-Wall -Wextra
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags) -fno-rtti
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR)
CLANGLIBS=\
	-Wl,--start-group\
	-lclang\
	-lclangFrontend\
	-lclangDriver\
	-lclangSerialization\
	-lclangParse\
	-lclangSema\
	-lclangAnalysis\
	-lclangEdit\
	-lclangAST\
	-lclangLex\
	-lclangBasic\
	-Wl,--end-group
LLVMLIBS=$(shell $(LLVM_CONFIG) --libs)
LIBFILES=$(shell $(LLVM_CONFIG) --libfiles)
SYSTEMLIBS=$(shell $(LLVM_CONFIG) --system-libs)

PROGRAM=func-meth-printer
PROGRAM_OBJECTS=program.o

default: $(PROGRAM)

%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PROGRAM) : $(PROGRAM_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(CLANGLIBS) $(LLVMLIBS) $(SYSTEMLIBS)

clean::
	$(QUIET)rm -f $(PROGRAM) $(PROGRAM_OBJECTS)

