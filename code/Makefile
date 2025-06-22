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

.PHONY: all clean compile test validate


##########################
##### MAKEFILE RULES #####
##########################
all:
	$(CALL_MAKE) _all

clean:
	$(CALL_MAKE) _clean

compile:
	$(CALL_MAKE) _compile

test:
	$(CALL_MAKE) _test

validate:
	$(CALL_MAKE) _validate
