##########################
###### INSTRUCTIONS ######
##########################
#
# 1. Stop reading this
# 2. Read Makefile instead
# 3. Thank Linus for this wonderful kernel!
#


##################################
#### LINUX MAKEFILE VARIABLES ####
##################################

### CONSTANT VARIABLES ###
# This was made to avoid circular dependencies and redundancies
include Makefile_constants.mk
# $(info $$DST_DIR is [${DST_DIR}])  # DEBUGGING

BIN_FILE_EXT = .bin
LIB_FILE_EXT = .lib
SHO_FILE_EXT = .so
# Defined this to aid in the proper exeuction of a list of commands
define execute-command
$(1)

endef

### *NIX DIRECTORIES ###
NIX_DELIM = /
# Name of the "code" directory
NIX_CODE_DIR = $(CODE_DIR)$(NIX_DELIM)
# Name of the header file directory
NIX_HDR_DIR = $(HDR_DIR)$(NIX_DELIM)
# Name of the source file directory
NIX_SRC_DIR = $(SRC_DIR)$(NIX_DELIM)
# Name of the distribution directory
NIX_DST_DIR = $(DST_DIR)$(NIX_DELIM)

### COMPILER VARIABLES ###
CC = gcc
AFLAGS = -fsanitize=address -g
CFLAGS = -Wall -Werror -Wfatal-errors

# $(NULL) - Shunt output here to silence it
NULL :=
ifneq ($(OS),Windows_NT)
	NULL = /dev/null
else
$(error Wrong operating system.  This is $(OS).)
endif

### DYNAMIC VARIABLES ###

# SOURCE FILES
# All .c filenames found in SRC_DIR
RAW_SRC_FILES := $(shell cd $(NIX_SRC_DIR); ls *$(SRC_FILE_EXT))
# All RAW_SRC_FILES with the file extension stripped
BASE_SRC_NAMES := $(basename $(RAW_SRC_FILES))
# Convert base filenames to object code filenames
RAW_OBJ_FILES := $(addsuffix $(OBJ_FILE_EXT),$(BASE_SRC_NAMES))

##################################
###### LINUX MAKEFILE RULES ######
##################################
_all:
	$(CALL_MAKE) _clean
	$(CALL_MAKE) _compile

.PHONY: _all _clean _clean_dist _compile

# Don't auto-remove my object code
.PRECIOUS: $(foreach RAW_OBJ_FILE, $(RAW_OBJ_FILES), $(NIX_DST_DIR)$(RAW_OBJ_FILE))

_clean:
	$(CALL_MAKE) _clean_dist

_clean_dist:
	@echo "    Cleaning "$(NIX_DST_DIR)" directory"
	@rm -f $(NIX_DST_DIR)*$(OBJ_FILE_EXT) $(NIX_DST_DIR)*$(BIN_FILE_EXT) $(NIX_DST_DIR)*$(LIB_FILE_EXT) $(NIX_DST_DIR)*$(SHO_FILE_EXT)

_compile: $(NIX_DST_DIR)sum_dock$(BIN_FILE_EXT)
	@#echo "$@ needs $^"  # DEBUGGING

$(NIX_DST_DIR)sum_dock$(BIN_FILE_EXT): $(foreach RAW_OBJ_FILE, $(RAW_OBJ_FILES), $(NIX_DST_DIR)$(RAW_OBJ_FILE))
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Linking SUM DOCK (SUDO) binary: $@"
	@$(CC) $(CFLAGS) -o $@ $^ -I $(NIX_HDR_DIR)

# DEFAULT: Linking binaries
$(NIX_DST_DIR)%$(BIN_FILE_EXT): $(NIX_DST_DIR)%$(OBJ_FILE_EXT)
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Linking binary: $@"
	@$(CC) $(CFLAGS) -o $@ $^

# DEFAULT: Compiling object code
$(NIX_DST_DIR)%$(OBJ_FILE_EXT): $(NIX_SRC_DIR)%$(SRC_FILE_EXT)
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Compiling: $^"
	@$(CC) $(CFLAGS) -c $^ -o $@ -I $(NIX_HDR_DIR)

# DEFAULT: Validating source files exist
%$(SRC_FILE_EXT):
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Verifying $@"
	@if ! [ -f $@ ] ; then echo "Unable to locate the $@ file" >&2 && exit 2 ; fi
