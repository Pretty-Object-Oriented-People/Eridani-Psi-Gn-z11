#
# Tools
#

#Cross platform fixes
ifeq ($(OS), Windows_NT)
    OSNAME := Windows
    SLLTYPE := lib
    DLLTYPE := dll
    ifndef WINGNUCORE
        ifneq (,$(wildcard ./tools/win/coreutils))
            WINGNUCORE := ./tools/win/coreutils/bin/
        endif
    endif
    ifndef WINGNUCORE
        LS := ls
        RM := rm -f
        MKDIR := mkdir -p
        MV := mv
    else
        LS := $(WINGNUCORE)ls.exe
        RM := $(WINGNUCORE)rm.exe -f
        MKDIR := $(WINGNUCORE)mkdir.exe -p
        MV := $(WINGNUCORE)mv.exe
    endif
    ifndef WINGNUSED
        ifneq (,$(wildcard ./tools/win/sed))
            WINGNUSED := ./tools/win/sed/bin/
        endif
    endif
    ifndef WINGNUSED
        SED := sed
    else
        SED := "$(WINGNUSED)sed.exe"
    endif
else
    OSNAME := Linux
    SLLTYPE := a
    DLLTYPE := so
    LS := ls
    RM := rm -f
    MKDIR := mkdir -p
    SED := sed
    MV := mv
endif
ifndef CARCH
    CARCH := x86_64
endif

#Recursive wildcard
rwildcard=$(foreach d,$1,$(wildcard $d$2) $(if $(wildcard $d*), $(call rwildcard,$(sort $(dir $(wildcard $d*/*))),$2)))

#
# Properties
#

include properties.make

.DEFAULT_GOAL := $(CMAIN)

#Directory structure declaration
SRC_DIR ?= src
INCL_DIR ?= inc
BUILDIR ?= build
OBJ_DIR ?= $(BUILDIR)
DEP_DIR ?= $(BUILDIR)
PRG_DIR ?= $(BUILDIR)
LIB_DIR ?= lib

LIBSf := $(foreach l,$(LIBS),-l$l) $(foreach l,$(LIBSp),-l:$l)

#Compiler options
CFLAGS += $(CFLAGSp)
release: CFLAGSo ?= -O2
test: CFLAGSo ?= -O2
debug: CFLAGSo ?= -O0 -ggdb -D __DEBUG_MODE
test_debug: CFLAGSo ?= -O0 -ggdb -D __DEBUG_MODE
lib: CFLAGSo ?= -O2
libd: CFLAGSo ?= -O2 -fPIC
CFLAGS += $(CFLAGSo)
CFLAGS += -std=$(CSTD)
CFLAGS += -I$(SRC_DIR) -I$(INCL_DIR)
ifeq ($(CSTD),gnu99)
    CFLAGS += -D __C99 -D __CSTD_GNU99
endif
ifeq ($(CSTD),c99)
    CFLAGS += -D __C99
endif
ifeq ($(CSTD),gnu11)
    CFLAGS += -D __C99 -D __C11 -D __CSTD_GNU99 -D __CSTD_GNU11
endif
ifeq ($(CSTD),c11)
    CFLAGS += -D __C99 -D __C11
endif

#
# Run
#

#Automatically find all source files and associate objects, deps, and progs
SOURCES := $(call rwildcard,$(SRC_DIR)/,*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
DEPENDENCIES := $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SOURCES))
PROGRAMS := $(patsubst $(SRC_DIR)/%.c, $(PRG_DIR)/%, $(SOURCES))

#Include d dep files, when needed
ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPENDENCIES)
endif

#Make dependency by getting all [non-system] prerequisites from src
$(DEP_DIR)/%.d: $(SRC_DIR)/%.c
	$(info Making dependency for: $@)
	@$(MKDIR) $(@D)
	@$(CC) -MM $(CFLAGS) $< | $(SED) "s;\($*\)\.o[ :]*;$(OBJ_DIR)/\1.o $@ : ;g" > $@

#Make object by compiling src
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(info Making object: $@)
	@$(MKDIR) $(@D)
	@$(CC) $< -o $@ -c $(CFLAGS)

#Linking program requires dependencies
$(PRG_DIR)/%: $(DEP_DIR)/%.d $(OBJECTS)
	$(info Making program: $@, Args: $(CFLAGS))
	@objcopy --redefine-sym main_$(@F)=main $@.o
	@$(CC) $(OBJECTS) -o $@ $(CFLAGS) -L$(LIB_DIR) -L$(LIB_DIR)/$(OSNAME) -L$(LIB_DIR)/$(CARCH) -L$(LIB_DIR)/$(OSNAME)/$(CARCH) $(LIBSf)
	@objcopy --redefine-sym main=main_$(@F) $@.o

$(PRG_DIR)/lib%.$(SLLTYPE): $(OBJECTS)
	$(info Making static library: $@, Args: $(CFLAGS))
	@ar rcs $@ $(OBJECTS)

ifeq ($(OSNAME), Windows)
$(PRG_DIR)/lib%.$(DLLTYPE): $(OBJECTS)
	$(info Making dynamic/shared library: $@, Args: $(CFLAGS))
	@$(CC) -shared $(OBJECTS) -o $@ -Wl,--out-implib,$@.a $(CFLAGS) -L$(LIB_DIR) -L$(LIB_DIR)/$(OSNAME) -L$(LIB_DIR)/$(CARCH) -L$(LIB_DIR)/$(OSNAME)/$(CARCH) $(LIBSf)
else
$(PRG_DIR)/lib%.$(DLLTYPE): $(OBJECTS)
	$(info Making dynamic/shared library: $@, Args: $(CFLAGS))
	@$(CC) -shared $(OBJECTS) -o $@ $(CFLAGS) -L$(LIB_DIR) -L$(LIB_DIR)/$(OSNAME) -L$(LIB_DIR)/$(CARCH) -L$(LIB_DIR)/$(OSNAME)/$(CARCH) $(LIBSf)
endif

#Specials
release: $(BUILDIR)/release
test: $(BUILDIR)/test
debug: $(BUILDIR)/debug
test_debug: $(BUILDIR)/test
lib: $(BUILDIR)/lib$(LIBNAME).$(SLLTYPE)
libd: $(BUILDIR)/lib$(LIBNAME).$(DLLTYPE)

all: printinfo release test lib

#Clean
clean:
	$(RM) $(BUILDIR)/* -R

printinfo:
	$(info OS: $(OS); Target Arch: $(CARCH))
	$(info Compiler: $(CC); Options: $(CFLAGS))
	$(info Windows GNU Core: $(WINGNUCORE); Windows GNU sed: $(WINGNUSED))
	$(info src: $(SRC_DIR); incl: $(INCL_DIR); libs: $(LIB_DIR))
	$(info deps: $(DEP_DIR); objects: $(OBJ_DIR); programs: $(PRG_DIR))
	$(info static libraries: $(LIBS))

#PHONY
.PHONY: all release test debug test_debug lib libd clean printinfo