I_DEBUG_FLAG ?= 0
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
srcdir := .

rlsdir := $(srcdir)/release
dbgdir := $(srcdir)/debug

ifeq ($(I_DEBUG_FLAG), 0)
	bindir := $(rlsdir)/bin
	objdir := $(rlsdir)/obj
	runtitle := Run Release
else
	bindir := $(dbgdir)/bin
	objdir := $(dbgdir)/obj
	runtitle := Run Debug
endif
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
appname := $(notdir $(shell pwd))
SRCS 	:= $(wildcard $(srcdir)/*.cpp)
OBJS 	:= $(addprefix $(objdir)/, $(patsubst %.cpp, %.o, $(notdir $(realpath \
																$(SRCS)))))
APP 	:= $(addprefix $(bindir)/, $(appname).exe)
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
CXX := g++

CXX_WARN_FLAGS 	:= -Wpedantic -Weffc++
CXX_GEN_FLAGS	:= -fPIE
CXX_CMN_FLAGS 	:= -c -std=c++11 $(CXX_WARN_FLAGS)
CXX_DEBUG_FLAGS := -ggdb
CXX_OPT_FLAGS 	:= -O2

CXX_FLAGS := $(CXX_CMN_FLAGS)

ifeq ($(I_DEBUG_FLAG), 0)
	CXX_FLAGS += $(CXX_OPT_FLAGS)
else
	CXX_FLAGS += $(CXX_DEBUG_FLAGS)
endif
#------------------------------------------------------------------------------#
LD := g++
LD_FLAGS := -fPIE
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
all: $(APP)

.PHONY: all

$(APP): $(OBJS) | $(bindir)
	$(LD) $^ -o $(APP)

$(OBJS): $(SRCS) | $(objdir)
	$(CXX) $(CXX_FLAGS) -o $@ $<

$(objdir) $(bindir):
	@$(foreach d,$(subst /, ,$@), test -d $d || mkdir $d && cd $d && ):
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
.PHONY: clean

clean:
	$(RM) $(APP)
	$(RM) $(OBJS)
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
.PHONY: run

run: $(APP)
	@echo ------------------------------
	@echo $(runtitle)
	@echo ------------------------------
	$(APP)
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
.PHONY: test

test: input.txt
	$(APP) < input.txt