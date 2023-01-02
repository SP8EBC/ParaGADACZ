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
../src/ForecastFinder_test.cpp \
../src/PlaylistCreatorPL.cpp \
../src/PlaylistCreatorPL_test.cpp 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprxLogParser.o \
./src/ConfigurationFile.o \
./src/ForecastFinder.o \
./src/ForecastFinder_test.o \
./src/PlaylistCreatorPL.o \
./src/PlaylistCreatorPL_test.o 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprxLogParser.d \
./src/ConfigurationFile.d \
./src/ForecastFinder.d \
./src/ForecastFinder_test.d \
./src/PlaylistCreatorPL.d \
./src/PlaylistCreatorPL_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


