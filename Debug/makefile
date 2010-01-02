################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Util/subdir.mk
-include Topo/subdir.mk
-include Tests/subdir.mk
-include Tess/Planar/subdir.mk
-include Subdivision/subdir.mk
-include Geom/Tests/subdir.mk
-include Geom/subdir.mk
-include Formats/FREP/subdir.mk
-include BREP_Algo/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: FreeREP

# Tool invocations
FreeREP: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -o"FreeREP" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(OBJS)$(CC_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(C++_DEPS) FreeREP
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
