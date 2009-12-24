################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Tess/Planar/construct.cpp \
../Tess/Planar/misc.cpp \
../Tess/Planar/monotone.cpp \
../Tess/Planar/tri.cpp 

CPP_DEPS += \
./Tess/Planar/construct.d \
./Tess/Planar/misc.d \
./Tess/Planar/monotone.d \
./Tess/Planar/tri.d 

OBJS += \
./Tess/Planar/construct.o \
./Tess/Planar/misc.o \
./Tess/Planar/monotone.o \
./Tess/Planar/tri.o 


# Each subdirectory must supply rules for building sources it contributes
Tess/Planar/%.o: ../Tess/Planar/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


