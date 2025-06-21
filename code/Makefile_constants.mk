##########################
###### INSTRUCTIONS ######
##########################
#
# 1. Stop reading this
# 2. Read Makefile instead
# 3. Take care to avoid circular inclusion and redundancies
#


##########################
### MAKEFILE VARIABLES ###
##########################

### DIRECTORIES ###
# Name of the "code" directory
CODE_DIR = code
# Name of the header file directory
HDR_DIR = include
# Name of the source file directory
SRC_DIR = src
# Name of the distribution directory
DST_DIR = dist

### OS-DYNAMIC VARIABLES ###
# $(CHECK) - Checkmark
CHECK :=
ifneq ($(OS),Windows_NT)
	CHECK = [✓]
else
	CHECK = [*]
endif

### FILE EXTENSIONS ###
HDR_FILE_EXT = .h
OBJ_FILE_EXT = .o
SRC_FILE_EXT = .c

### MAKEFILE ARGUMENTS ###
MF_ARGS = --no-print-directory
CALL_MAKE = @$(MAKE) $(MF_ARGS)
