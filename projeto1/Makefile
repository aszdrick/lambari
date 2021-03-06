###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

################################## VARIABLES ##################################
# Directories
SRCDIR   :=src
HDRDIR   :=include
OBJDIR   :=build
BINDIR   :=.
TESTDIR  :=tests
DEPDIR   :=.deps
MAKEDIR  :=$(BINDIR) $(SRCDIR) $(HDRDIR)
# Compiler & linker flags
CXX       :=clang++
LDLIBS    :=-lgtest -pthread
LDFLAGS   :=
CXXFLAGS  :=-std=c++14 -fcxx-exceptions -Wno-deprecated-register -Wall\
            -Wno-unused-function -Wno-unneeded-internal-declaration
INCLUDE  :=-I$(HDRDIR)
# MaKefile eXtension variables
MKXFILES :=$(wildcard *.mkx)
MKXCLEAN :=$(patsubst %.mkx,clean_%,$(MKXFILES))
# Files
SRC      :=$(shell find $(SRCDIR) -name '*.cpp')
DEP      :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(SRC))
OBJ      :=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
MAIN     :=main
EXEC     :=$(BINDIR)/lukacompiler
# Test files
PUREOBJ  :=$(filter-out $(OBJDIR)/$(SRCDIR)/$(MAIN).o,$(OBJ))
TSRC     :=$(shell find $(TESTDIR) -name '*.cpp' 2> /dev/null)
TMAIN    :=$(wildcard $(TESTDIR)/*.cpp)
TDEP     :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(TSRC))
TOBJ     :=$(patsubst %.cpp,$(OBJDIR)/%.o,$(TSRC))
TPUREOBJ :=$(filter-out $(patsubst %.cpp,$(OBJDIR)/%.o,$(TMAIN)),$(TOBJ))
TEXEC    :=$(patsubst $(TESTDIR)/%.cpp,$(BINDIR)/%,$(TMAIN))
TCALL    :=$(patsubst %.cpp,%,$(notdir $(TMAIN)))

.PHONY: all makedir clean clean_deps clean_all tests shit

################################# MAIN RULES ##################################
all: makedir $(EXEC)

-include $(MKXFILES)

$(EXEC): $(OBJ)
	@echo "[linking] $@"
	@$(CXX) $(OBJ) -o $@ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	@echo "[$(CXX)] $< -> .o"
	@mkdir -p $(OBJDIR)/$(*D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# For each .cpp file, creates a .d file with all dependencies of .cpp,
# including .d as target (to ensure up-to-date dependencies, in case of
# new includes being added)
$(DEPDIR)/%.d: %.cpp
	@echo "[makedep] $< -> .d"
	@mkdir -p $(DEPDIR)/$(*D)
	@$(CXX) -MM -MP -MT "$(OBJDIR)/$*.o $@" -MF "$@" $< $(INCLUDE) $(CXXFLAGS)

makedir: | $(MAKEDIR)

$(MAKEDIR):
	@echo "[ mkdir ] Creating directory '$@'"
	@mkdir -p $@

################################ TESTS RULES ##################################
tests: makedir $(TCALL)

#$(foreach t,$(TCALL),$(t): $(OBJDIR)/$(TESTDIR)/$(t).o $(BINDIR)/$(t))
$(foreach t,$(TCALL),$(BINDIR)/$(t): $(OBJDIR)/$(TESTDIR)/$(t).o)

$(TEXEC): $(PUREOBJ) $(TPUREOBJ)
	@echo "[linking] $@"
	@$(CXX) $(PUREOBJ) $(TPUREOBJ) $(OBJDIR)/$(TESTDIR)/$(@F).o \
	-o $@ $(LDLIBS) $(LDFLAGS)

################################ CLEAN RULES ##################################
# Only remove object files
clean: $(MKXCLEAN)
	@$(RM) -r $(OBJDIR)

# Remove object, binary and dependency files
clean_all: clean
	@$(RM) $(EXEC) $(TEXEC)
	@$(RM) -r $(DEPDIR)

################################ PREREQUISITES ################################
# Do not include list of dependencies when they are going to be deleted, i.e.,
# when the target is clean_all
ifneq ($(MAKECMDGOALS), clean_all)
  -include $(DEP)
  ifeq ($(MAKECMDGOALS), tests)
    -include $(TDEP)
  endif
endif
