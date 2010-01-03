################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Subdivision/MaximumEdgeLength.cpp \
../Subdivision/Sub_MaxEdgeLength.cpp 

CPP_DEPS += \
./Subdivision/MaximumEdgeLength.d \
./Subdivision/Sub_MaxEdgeLength.d 

OBJS += \
./Subdivision/MaximumEdgeLength.o \
./Subdivision/Sub_MaxEdgeLength.o 


# Each subdirectory must supply rules for building sources it contributes
Subdivision/%.o: ../Subdivision/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


