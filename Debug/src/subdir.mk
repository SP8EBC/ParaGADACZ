################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AprsPacket.cpp \
../src/AprsWXData.cpp \
../src/AprxLogParser.cpp \
../src/ConfigurationFile.cpp \
../src/ForecastFinder.cpp \
../src/PlaylistAssembler.cpp \
../src/PlaylistAssembler_test.cpp \
../src/PlaylistSamplerPL.cpp \
../src/main.cpp 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprxLogParser.o \
./src/ConfigurationFile.o \
./src/ForecastFinder.o \
./src/PlaylistAssembler.o \
./src/PlaylistAssembler_test.o \
./src/PlaylistSamplerPL.o \
./src/main.o 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprxLogParser.d \
./src/ConfigurationFile.d \
./src/ForecastFinder.d \
./src/PlaylistAssembler.d \
./src/PlaylistAssembler_test.d \
./src/PlaylistSamplerPL.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


