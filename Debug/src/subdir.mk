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
../src/ConfigurationFile.cpp \
../src/CurrentConditionsDownloader.cpp \
../src/ForecastDownloader.cpp \
../src/ForecastDownloader_test_index.cpp \
../src/ForecastFinder.cpp \
../src/InhibitorAndPttControl.cpp \
../src/Player.cpp \
../src/PlaylistAssembler.cpp \
../src/PlaylistSamplerPL.cpp \
../src/PogodaccDownloader.cpp \
../src/TimeTools.cpp \
../src/WeatherlinkDownloader.cpp 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprsWXDataFactory.o \
./src/AprxLogParser.o \
./src/AvalancheWarnings.o \
./src/ConfigurationFile.o \
./src/CurrentConditionsDownloader.o \
./src/ForecastDownloader.o \
./src/ForecastDownloader_test_index.o \
./src/ForecastFinder.o \
./src/InhibitorAndPttControl.o \
./src/Player.o \
./src/PlaylistAssembler.o \
./src/PlaylistSamplerPL.o \
./src/PogodaccDownloader.o \
./src/TimeTools.o \
./src/WeatherlinkDownloader.o 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprsWXDataFactory.d \
./src/AprxLogParser.d \
./src/AvalancheWarnings.d \
./src/ConfigurationFile.d \
./src/CurrentConditionsDownloader.d \
./src/ForecastDownloader.d \
./src/ForecastDownloader_test_index.d \
./src/ForecastFinder.d \
./src/InhibitorAndPttControl.d \
./src/Player.d \
./src/PlaylistAssembler.d \
./src/PlaylistSamplerPL.d \
./src/PogodaccDownloader.d \
./src/TimeTools.d \
./src/WeatherlinkDownloader.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


