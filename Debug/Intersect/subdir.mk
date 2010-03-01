################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Intersect/SimpleIntersector.cpp 

CPP_DEPS += \
./Intersect/SimpleIntersector.d 

OBJS += \
./Intersect/SimpleIntersector.o 


# Each subdirectory must supply rules for building sources it contributes
Intersect/%.o: ../Intersect/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


