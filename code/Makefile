##########################
###### INSTRUCTIONS ######
##########################
#
# 1. Stop reading this
# 2. Read Makefile instead
# 3. Profit
#


##########################
### MAKEFILE VARIABLES ###
##########################

### OS-DEPENDENT INCLUSION ###
ifeq ($(OS),Windows_NT)
include Makefile_windows.mk
else
include Makefile_linux.mk
endif

.PHONY: all clean compile


##########################
##### MAKEFILE RULES #####
##########################
all:
	$(CALL_MAKE) _clean
	$(CALL_MAKE) _compile

clean:
	$(CALL_MAKE) _clean

compile:
	$(CALL_MAKE) _compile
