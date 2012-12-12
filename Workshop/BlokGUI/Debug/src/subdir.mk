################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlokGUI_Command.cpp \
../src/BlokGUI_Render.cpp \
../src/Font.cpp 

OBJS += \
./src/BlokGUI_Command.o \
./src/BlokGUI_Render.o \
./src/Font.o 

CPP_DEPS += \
./src/BlokGUI_Command.d \
./src/BlokGUI_Render.d \
./src/Font.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/local/include/freetype2 -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


