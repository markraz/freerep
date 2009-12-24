################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Topo/Topo_Arc.cpp \
../Topo/Topo_Edge.cpp \
../Topo/Topo_Face.cpp \
../Topo/Topo_Face_Conic.cpp \
../Topo/Topo_Line.cpp \
../Topo/Topo_Solid.cpp \
../Topo/Topo_Wire.cpp 

CPP_DEPS += \
./Topo/Topo_Arc.d \
./Topo/Topo_Edge.d \
./Topo/Topo_Face.d \
./Topo/Topo_Face_Conic.d \
./Topo/Topo_Line.d \
./Topo/Topo_Solid.d \
./Topo/Topo_Wire.d 

OBJS += \
./Topo/Topo_Arc.o \
./Topo/Topo_Edge.o \
./Topo/Topo_Face.o \
./Topo/Topo_Face_Conic.o \
./Topo/Topo_Line.o \
./Topo/Topo_Solid.o \
./Topo/Topo_Wire.o 


# Each subdirectory must supply rules for building sources it contributes
Topo/%.o: ../Topo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


