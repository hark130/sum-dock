##########################
###### INSTRUCTIONS ######
##########################
#
# 1. make
# 2. TO DO: DON'T DO NOW... binary usage goes here
# 3. profit
#
# Global "constants" are defined in Makefile_constants
# Source: https://github.com/hark130/sum-dock
#


##########################
### MAKEFILE VARIABLES ###
##########################

### DIRECTORIES ###
# Relative path to the directory holding the code and its Makefile
CODE_DIR = code

### MAKEFILE ARGUMENTS ###
MF_ARGS = --directory=$(CODE_DIR)
CALL_MAKE = @$(MAKE) --no-print-directory

.PHONY: all compile clean test validate


##########################
##### MAKEFILE RULES #####
##########################
all:
	$(CALL_MAKE) validate
	$(CALL_MAKE) clean
	$(CALL_MAKE) compile
	$(CALL_MAKE) test

compile:
	@echo ""
	@echo "COMPILING"
	$(CALL_MAKE) $(MF_ARGS) compile

clean:
	@echo ""
	@echo "CLEANING"
	$(CALL_MAKE) $(MF_ARGS) clean

test:
	@echo ""
	@echo "TESTING"
	$(CALL_MAKE) $(MF_ARGS) test

validate:
	@echo ""
	@echo "VALIDATING"
	$(CALL_MAKE) $(MF_ARGS) validate
