#******************************************************************************
#                *          ***                    ***
#              ***          ***                    ***
# ***  ****  **********     ***        *****       ***  ****          *****
# *********  **********     ***      *********     ************     *********
# ****         ***          ***              ***   ***       ****   ***
# ***          ***  ******  ***      ***********   ***        ****   *****
# ***          ***  ******  ***    *************   ***        ****      *****
# ***          ****         ****   ***       ***   ***       ****          ***
# ***           *******      ***** **************  *************    *********
# ***             *****        ***   *******   **  **  ******         *****
#                           t h e  r e a l t i m e  t a r g e t  e x p e r t s
#
# http://www.rt-labs.com
# Copyright (C) 2006. rt-labs AB, Sweden. All rights reserved.
#------------------------------------------------------------------------------
# $Id: app.mk 125 2012-04-01 17:36:17Z rtlaka $
#------------------------------------------------------------------------------

OBJDIR = obj/$(ARCH)
ifeq ($(ARCH),linux)
LIBS += pthread
endif
LIBS += oshw osal soem

include $(PRJ_ROOT)/make/rules.mk
include $(PRJ_ROOT)/make/files.mk

SUBDIRS = $(patsubst %/,%,$(dir $(wildcard */Makefile)))

# Use .PHONY so link step always occurs. This is a simple way
# to avoid computing dependencies on libs
.PHONY: $(APPNAME)
$(APPNAME): $(OBJDIR) $(SUBDIRS) $(OBJDIR_OBJS) $(EXTRA_OBJS)
	@echo --- Linking $@
	$(SILENT)$(CC) $(LDFLAGS) $(LD_PATHS) $(OBJDIR)/*.o $(LIBS) -o $@


$(OBJDIR):
	@test -e $(OBJDIR) || $(MKDIR) $(OBJDIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo --- Entering $(CURDIR)/$@
	@$(MAKE) -C $@ $(MAKECMDGOALS)
	@echo --- Leaving $(CURDIR)/$@

.PHONY: clean
clean: $(SUBDIRS)
	@$(RM) $(OBJDIR)/*
	@$(RM) $(APPNAME)
	@$(RM) $(APPNAME).elf 
	@$(RM) $(APPNAME).map

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
