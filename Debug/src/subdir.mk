################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AprsPacket.cpp \
../src/AprsWXData.cpp \
../src/AprsWXDataFactory.cpp \
../src/AprxLogParser.cpp \
../src/AvalancheWarnings.cpp \
../src/AvalancheWarnings_test.cpp \
../src/ConfigurationFile.cpp \
../src/CurentConditionsDownloader.cpp \
../src/ForecastDownloader.cpp \
../src/ForecastFinder.cpp \
../src/InhibitorAndPttControl.cpp \
../src/Player.cpp \
../src/PlaylistAssembler.cpp \
../src/PlaylistSamplerPL.cpp 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprsWXDataFactory.o \
./src/AprxLogParser.o \
./src/AvalancheWarnings.o \
./src/AvalancheWarnings_test.o \
./src/ConfigurationFile.o \
./src/CurentConditionsDownloader.o \
./src/ForecastDownloader.o \
./src/ForecastFinder.o \
./src/InhibitorAndPttControl.o \
./src/Player.o \
./src/PlaylistAssembler.o \
./src/PlaylistSamplerPL.o 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprsWXDataFactory.d \
./src/AprxLogParser.d \
./src/AvalancheWarnings.d \
./src/AvalancheWarnings_test.d \
./src/ConfigurationFile.d \
./src/CurentConditionsDownloader.d \
./src/ForecastDownloader.d \
./src/ForecastFinder.d \
./src/InhibitorAndPttControl.d \
./src/Player.d \
./src/PlaylistAssembler.d \
./src/PlaylistSamplerPL.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


