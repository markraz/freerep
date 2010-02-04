################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Geom/Tests/Geom_Matrix_Test.cpp \
../Geom/Tests/Geom_Plane_Test.cpp \
../Geom/Tests/Geom_Test.cpp \
../Geom/Tests/Geom_Vec3_Test.cpp 

CPP_DEPS += \
./Geom/Tests/Geom_Matrix_Test.d \
./Geom/Tests/Geom_Plane_Test.d \
./Geom/Tests/Geom_Test.d \
./Geom/Tests/Geom_Vec3_Test.d 

OBJS += \
./Geom/Tests/Geom_Matrix_Test.o \
./Geom/Tests/Geom_Plane_Test.o \
./Geom/Tests/Geom_Test.o \
./Geom/Tests/Geom_Vec3_Test.o 


# Each subdirectory must supply rules for building sources it contributes
Geom/Tests/%.o: ../Geom/Tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


