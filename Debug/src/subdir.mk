################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AprsPacket.cpp \
../src/AprsWXData.cpp \
../src/AprxLogParser.cpp \
../src/AprxLogParser_test.cpp 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprxLogParser.o \
./src/AprxLogParser_test.o 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprxLogParser.d \
./src/AprxLogParser_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I../lib/nlohman_json_3_7_1/include/ -I../pogodacc -I../pogodacc/model -I/usr/include/curl -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


