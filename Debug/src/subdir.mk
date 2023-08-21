################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AprsPacket.cpp \
../src/AprsWXData.cpp \
../src/AprsWXDataFactory.cpp \
../src/AprxLogParser.cpp \
../src/AprxLogParserStaticStuff.cpp \
../src/AprxLogParser_test.cpp \
../src/AvalancheWarnings.cpp \
../src/ConfigurationFile.cpp \
../src/CurrentConditionsDownloader.cpp \
../src/ForecastDownloader.cpp \
../src/ForecastFinder.cpp \
../src/InhibitorAndPttControl.cpp \
../src/MeteobluePictocode.cpp \
../src/MeteoblueRainParser.cpp \
../src/Player.cpp \
../src/PlaylistAssembler.cpp \
../src/PlaylistSamplerPL.cpp \
../src/PogodaccDownloader.cpp \
../src/TimeTools.cpp \
../src/TrendDownloader.cpp \
../src/WeatherlinkDownloader.cpp 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprsWXDataFactory.d \
./src/AprxLogParser.d \
./src/AprxLogParserStaticStuff.d \
./src/AprxLogParser_test.d \
./src/AvalancheWarnings.d \
./src/ConfigurationFile.d \
./src/CurrentConditionsDownloader.d \
./src/ForecastDownloader.d \
./src/ForecastFinder.d \
./src/InhibitorAndPttControl.d \
./src/MeteobluePictocode.d \
./src/MeteoblueRainParser.d \
./src/Player.d \
./src/PlaylistAssembler.d \
./src/PlaylistSamplerPL.d \
./src/PogodaccDownloader.d \
./src/TimeTools.d \
./src/TrendDownloader.d \
./src/WeatherlinkDownloader.d 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprsWXDataFactory.o \
./src/AprxLogParser.o \
./src/AprxLogParserStaticStuff.o \
./src/AprxLogParser_test.o \
./src/AvalancheWarnings.o \
./src/ConfigurationFile.o \
./src/CurrentConditionsDownloader.o \
./src/ForecastDownloader.o \
./src/ForecastFinder.o \
./src/InhibitorAndPttControl.o \
./src/MeteobluePictocode.o \
./src/MeteoblueRainParser.o \
./src/Player.o \
./src/PlaylistAssembler.o \
./src/PlaylistSamplerPL.o \
./src/PogodaccDownloader.o \
./src/TimeTools.o \
./src/TrendDownloader.o \
./src/WeatherlinkDownloader.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AprsPacket.d ./src/AprsPacket.o ./src/AprsWXData.d ./src/AprsWXData.o ./src/AprsWXDataFactory.d ./src/AprsWXDataFactory.o ./src/AprxLogParser.d ./src/AprxLogParser.o ./src/AprxLogParserStaticStuff.d ./src/AprxLogParserStaticStuff.o ./src/AprxLogParser_test.d ./src/AprxLogParser_test.o ./src/AvalancheWarnings.d ./src/AvalancheWarnings.o ./src/ConfigurationFile.d ./src/ConfigurationFile.o ./src/CurrentConditionsDownloader.d ./src/CurrentConditionsDownloader.o ./src/ForecastDownloader.d ./src/ForecastDownloader.o ./src/ForecastFinder.d ./src/ForecastFinder.o ./src/InhibitorAndPttControl.d ./src/InhibitorAndPttControl.o ./src/MeteobluePictocode.d ./src/MeteobluePictocode.o ./src/MeteoblueRainParser.d ./src/MeteoblueRainParser.o ./src/Player.d ./src/Player.o ./src/PlaylistAssembler.d ./src/PlaylistAssembler.o ./src/PlaylistSamplerPL.d ./src/PlaylistSamplerPL.o ./src/PogodaccDownloader.d ./src/PogodaccDownloader.o ./src/TimeTools.d ./src/TimeTools.o ./src/TrendDownloader.d ./src/TrendDownloader.o ./src/WeatherlinkDownloader.d ./src/WeatherlinkDownloader.o

.PHONY: clean-src

