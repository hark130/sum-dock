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
# Name of the distribution directory
NIX_TEST_DIR = $(TEST_DIR)$(NIX_DELIM)

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
# All .c filenames found in NIX_SRC_DIR
RAW_SRC_FILES := $(shell cd $(NIX_SRC_DIR); ls *$(SRC_FILE_EXT))
# All RAW_SRC_FILES with the file extension stripped
BASE_SRC_NAMES := $(basename $(RAW_SRC_FILES))
# Convert base filenames to object code filenames
RAW_OBJ_FILES := $(addsuffix $(OBJ_FILE_EXT),$(BASE_SRC_NAMES))
# All sudo*.c filenames found in NIX_SRC_DIR
SUDO_SRC_FILES := $(shell cd $(NIX_SRC_DIR); ls sudo*$(SRC_FILE_EXT))
# All SUDO_SRC_FILES with the file extension stripped
SUDO_SRC_NAMES := $(basename $(SUDO_SRC_FILES))
# Convert base filenames to object code filenames
SUDO_OBJ_FILES := $(addsuffix $(OBJ_FILE_EXT),$(SUDO_SRC_NAMES))

# CHECK UNIT TEST VARIABLES
# Prefix for *all* Check unit test files
CHECK_PREFIX = check_
# All check*.c filenames found in NIX_TEST_DIR
CHECK_SRC_FILES := $(shell cd $(NIX_TEST_DIR); ls $(CHECK_PREFIX)*$(SRC_FILE_EXT))
# All CHECK_SRC_FILES with the file extension stripped
CHECK_BASE_NAMES := $(basename $(CHECK_SRC_FILES))
# Convert base Check filenames to object code filenames
CHECK_OBJ_FILES := $(addsuffix $(OBJ_FILE_EXT),$(CHECK_BASE_NAMES))
# Convert base Check filenames to object code filenames
CHECK_BIN_FILES := $(addsuffix $(BIN_FILE_EXT),$(CHECK_BASE_NAMES))
# A space-separated list of all the test binaries
CHECK_BIN_LIST = $(shell ls $(NIX_DST_DIR)$(CHECK_PREFIX)*$(BIN_FILE_EXT))
# Check unit test library arguments for $(CC)
CHECK_CC_ARGS = -lcheck -lm -lsubunit -lrt -lpthread

##################################
###### LINUX MAKEFILE RULES ######
##################################
_all:
	$(CALL_MAKE) _validate
	$(CALL_MAKE) _clean
	$(CALL_MAKE) _compile
	$(CALL_MAKE) _test

.PHONY: _all _check_link _clean _clean_dist _compile _test _validate _validate_check _validate_gcc

# Don't auto-remove my object code
.PRECIOUS: $(foreach RAW_OBJ_FILE, $(RAW_OBJ_FILES), $(NIX_DST_DIR)$(RAW_OBJ_FILE))

# Link all of the Check unit test binaries
_check_link: $(foreach CHECK_BIN_FILE, $(CHECK_BIN_FILES), $(NIX_DST_DIR)$(CHECK_BIN_FILE))
	@#echo "$@ needs $^"  # DEBUGGING

_clean:
	$(CALL_MAKE) _clean_dist

_clean_dist:
	@echo "    Cleaning "$(NIX_DST_DIR)" directory"
	@rm -f $(NIX_DST_DIR)*$(OBJ_FILE_EXT) $(NIX_DST_DIR)*$(BIN_FILE_EXT) $(NIX_DST_DIR)*$(LIB_FILE_EXT) $(NIX_DST_DIR)*$(SHO_FILE_EXT)

_compile: $(NIX_DST_DIR)sum_dock$(BIN_FILE_EXT)
	@#echo "$@ needs $^"  # DEBUGGING
	$(CALL_MAKE) _check_link

_test:
	@#echo "$@ needs $^"  # DEBUGGING
	$(foreach CHECK_BIN_FILE, $(CHECK_BIN_LIST), $(call execute-command,./$(CHECK_BIN_FILE)))

_validate:
	$(CALL_MAKE) _validate_gcc
	$(CALL_MAKE) _validate_check

_validate_check:
	@echo "    Validating Check"
	@grep "Check" /usr/include/check.h > $(NULL)
	@echo "        $(CHECK) $(shell echo -n "Check unit test version: "; printf "%s %s %s\n" CHECK_MAJOR_VERSION CHECK_MINOR_VERSION CHECK_MICRO_VERSION | gcc -include check.h -E - | tail -n 1)"

_validate_gcc:
	@echo "    Validating "$(CC)""
	@$(CC) --version > $(NULL)
	@echo "        $(CHECK) $(shell $(CC) --version | head -n 1)"

$(NIX_DST_DIR)sum_dock$(BIN_FILE_EXT): $(NIX_DST_DIR)sum_dock$(OBJ_FILE_EXT) $(foreach SUDO_OBJ_FILE, $(SUDO_OBJ_FILES), $(NIX_DST_DIR)$(SUDO_OBJ_FILE))
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Linking SUM DOCK (SUDO) binary: $@"
	@$(CC) $(CFLAGS) -o $@ $^ -I $(NIX_HDR_DIR)

# CHECK: Linking unit test binaries
$(NIX_DST_DIR)$(CHECK_PREFIX)%$(BIN_FILE_EXT): $(NIX_DST_DIR)$(CHECK_PREFIX)%$(OBJ_FILE_EXT) $(foreach SUDO_OBJ_FILE, $(SUDO_OBJ_FILES), $(NIX_DST_DIR)$(SUDO_OBJ_FILE)) $(NIX_DST_DIR)unit_test_code$(OBJ_FILE_EXT)
	@echo "$@ needs $^"  # DEBUGGING
	@echo "    Linking Check unit test binary: $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(CHECK_CC_ARGS) -I $(NIX_HDR_DIR)

# CHECK: Compiling unit test object code
$(NIX_DST_DIR)$(CHECK_PREFIX)%$(OBJ_FILE_EXT): $(NIX_TEST_DIR)$(CHECK_PREFIX)%$(SRC_FILE_EXT)
	@#echo "$@ needs $^"  # DEBUGGING
	@echo "    Compiling Check unit test code: $^"
	@$(CC) $(CFLAGS) -c $^ -o $@ -I $(NIX_HDR_DIR)

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
