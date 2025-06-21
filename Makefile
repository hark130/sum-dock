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

.PHONY: all compile clean


##########################
##### MAKEFILE RULES #####
##########################
all:
	$(CALL_MAKE) clean
	$(CALL_MAKE) compile

compile:
	@echo ""
	@echo "COMPILING"
	$(CALL_MAKE) $(MF_ARGS) compile

clean:
	@echo ""
	@echo "CLEANING"
	$(CALL_MAKE) $(MF_ARGS) clean
