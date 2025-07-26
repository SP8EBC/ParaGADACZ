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
../src/AvalancheWarnings.cpp \
../src/ConfigurationFile.cpp \
../src/ConfigurationFile_Parsers.cpp \
../src/CurrentConditionsDownloader.cpp \
../src/EmailDownloader.cpp \
../src/EmailDownloaderMessage.cpp \
../src/ForecastDownloader.cpp \
../src/ForecastFinder.cpp \
../src/InhibitorAndPttControl.cpp \
../src/MeteobluePictocode.cpp \
../src/MeteoblueRainParser.cpp \
../src/PansaAirspace.cpp \
../src/Player.cpp \
../src/PlaylistAssembler.cpp \
../src/PlaylistAssemblerAirspace.cpp \
../src/PlaylistSamplerPL.cpp \
../src/PogodaccDownloader.cpp \
../src/ReturnValues.cpp \
../src/SpeechSynthesis.cpp \
../src/SpeechSynthesisResponsivevoice.cpp \
../src/SpeechSynthesisStaticStuff.cpp \
../src/TimeTools.cpp \
../src/TrendDownloader.cpp \
../src/WeatherlinkDownloader.cpp \
../src/main.cpp 

CPP_DEPS += \
./src/AprsPacket.d \
./src/AprsWXData.d \
./src/AprsWXDataFactory.d \
./src/AprxLogParser.d \
./src/AprxLogParserStaticStuff.d \
./src/AvalancheWarnings.d \
./src/ConfigurationFile.d \
./src/ConfigurationFile_Parsers.d \
./src/CurrentConditionsDownloader.d \
./src/EmailDownloader.d \
./src/EmailDownloaderMessage.d \
./src/ForecastDownloader.d \
./src/ForecastFinder.d \
./src/InhibitorAndPttControl.d \
./src/MeteobluePictocode.d \
./src/MeteoblueRainParser.d \
./src/PansaAirspace.d \
./src/Player.d \
./src/PlaylistAssembler.d \
./src/PlaylistAssemblerAirspace.d \
./src/PlaylistSamplerPL.d \
./src/PogodaccDownloader.d \
./src/ReturnValues.d \
./src/SpeechSynthesis.d \
./src/SpeechSynthesisResponsivevoice.d \
./src/SpeechSynthesisStaticStuff.d \
./src/TimeTools.d \
./src/TrendDownloader.d \
./src/WeatherlinkDownloader.d \
./src/main.d 

OBJS += \
./src/AprsPacket.o \
./src/AprsWXData.o \
./src/AprsWXDataFactory.o \
./src/AprxLogParser.o \
./src/AprxLogParserStaticStuff.o \
./src/AvalancheWarnings.o \
./src/ConfigurationFile.o \
./src/ConfigurationFile_Parsers.o \
./src/CurrentConditionsDownloader.o \
./src/EmailDownloader.o \
./src/EmailDownloaderMessage.o \
./src/ForecastDownloader.o \
./src/ForecastFinder.o \
./src/InhibitorAndPttControl.o \
./src/MeteobluePictocode.o \
./src/MeteoblueRainParser.o \
./src/PansaAirspace.o \
./src/Player.o \
./src/PlaylistAssembler.o \
./src/PlaylistAssemblerAirspace.o \
./src/PlaylistSamplerPL.o \
./src/PogodaccDownloader.o \
./src/ReturnValues.o \
./src/SpeechSynthesis.o \
./src/SpeechSynthesisResponsivevoice.o \
./src/SpeechSynthesisStaticStuff.o \
./src/TimeTools.o \
./src/TrendDownloader.o \
./src/WeatherlinkDownloader.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AprsPacket.d ./src/AprsPacket.o ./src/AprsWXData.d ./src/AprsWXData.o ./src/AprsWXDataFactory.d ./src/AprsWXDataFactory.o ./src/AprxLogParser.d ./src/AprxLogParser.o ./src/AprxLogParserStaticStuff.d ./src/AprxLogParserStaticStuff.o ./src/AvalancheWarnings.d ./src/AvalancheWarnings.o ./src/ConfigurationFile.d ./src/ConfigurationFile.o ./src/ConfigurationFile_Parsers.d ./src/ConfigurationFile_Parsers.o ./src/CurrentConditionsDownloader.d ./src/CurrentConditionsDownloader.o ./src/EmailDownloader.d ./src/EmailDownloader.o ./src/EmailDownloaderMessage.d ./src/EmailDownloaderMessage.o ./src/ForecastDownloader.d ./src/ForecastDownloader.o ./src/ForecastFinder.d ./src/ForecastFinder.o ./src/InhibitorAndPttControl.d ./src/InhibitorAndPttControl.o ./src/MeteobluePictocode.d ./src/MeteobluePictocode.o ./src/MeteoblueRainParser.d ./src/MeteoblueRainParser.o ./src/PansaAirspace.d ./src/PansaAirspace.o ./src/Player.d ./src/Player.o ./src/PlaylistAssembler.d ./src/PlaylistAssembler.o ./src/PlaylistAssemblerAirspace.d ./src/PlaylistAssemblerAirspace.o ./src/PlaylistSamplerPL.d ./src/PlaylistSamplerPL.o ./src/PogodaccDownloader.d ./src/PogodaccDownloader.o ./src/ReturnValues.d ./src/ReturnValues.o ./src/SpeechSynthesis.d ./src/SpeechSynthesis.o ./src/SpeechSynthesisResponsivevoice.d ./src/SpeechSynthesisResponsivevoice.o ./src/SpeechSynthesisStaticStuff.d ./src/SpeechSynthesisStaticStuff.o ./src/TimeTools.d ./src/TimeTools.o ./src/TrendDownloader.d ./src/TrendDownloader.o ./src/WeatherlinkDownloader.d ./src/WeatherlinkDownloader.o ./src/main.d ./src/main.o

.PHONY: clean-src

