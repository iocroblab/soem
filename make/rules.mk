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
# $Id: rules.mk 175 2012-06-21 07:14:12Z rtlaka $
#------------------------------------------------------------------------------

# Compiler specific settings
include $(PRJ_ROOT)/make/compilers/$(CROSS_GCC)-gcc.mk

# Include paths
DEFAULT_INC_PATH += $(PRJ_ROOT)/osal/$(BSP)
DEFAULT_INC_PATH += $(PRJ_ROOT)/oshw/$(BSP)
DEFAULT_INC_PATH += $(PRJ_ROOT)/soem

INC_PATH = $(DEFAULT_INC_PATH) $(CC_INC_PATH) $(EXTRA_INCLUDES)
INC_PATHS = $(patsubst %,-I"%",$(INC_PATH))

# Library paths
LD_PATH += $(LIBDIR)
LD_PATH += $(EXTRA_LD_PATHS)

LD_PATHS = $(patsubst %,-L%,$(LD_PATH))

# Dependency generation
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

# Common settings
CFLAGS += -DEVENT_INSTRUMENT -g -O0 -ffunction-sections
LDFLAGS += -Wl,--gc-sections

# Command-line overrides
CFLAGS   += $(EXTRA_CFLAGS)
CPPFLAGS += $(EXTRA_CPPFLAGS)
LDFLAGS  += $(EXTRA_LDFLAGS)


SILENT=@

# Rules

.SUFFIXES:

$(OBJDIR)/%.o: %.S
	@echo --- Assembling $<
	$(SILENT)$(CC) $(CFLAGS) $(INC_PATHS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo --- Compiling $<
	$(SILENT)$(CC) $(CFLAGS) $(INC_PATHS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@echo --- Compiling $<
	$(SILENT)$(CC) $(CFLAGS) $(CPPFLAGS) $(INC_PATHS) -c $< -o $@

