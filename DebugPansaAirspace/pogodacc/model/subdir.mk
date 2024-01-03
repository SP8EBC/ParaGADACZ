################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pogodacc/model/AvailableParametersModel.cpp \
../pogodacc/model/LocaleEntry.cpp \
../pogodacc/model/QualityFactor.cpp \
../pogodacc/model/StationCurrentLocalTime.cpp \
../pogodacc/model/StationDataModel.cpp \
../pogodacc/model/StationDefinitionModel.cpp \
../pogodacc/model/StationGroupStations.cpp \
../pogodacc/model/StationGroupStationsStation.cpp \
../pogodacc/model/StationsGroup.cpp \
../pogodacc/model/Status.cpp \
../pogodacc/model/Summary.cpp \
../pogodacc/model/Trend.cpp \
../pogodacc/model/TrendData.cpp 

CPP_DEPS += \
./pogodacc/model/AvailableParametersModel.d \
./pogodacc/model/LocaleEntry.d \
./pogodacc/model/QualityFactor.d \
./pogodacc/model/StationCurrentLocalTime.d \
./pogodacc/model/StationDataModel.d \
./pogodacc/model/StationDefinitionModel.d \
./pogodacc/model/StationGroupStations.d \
./pogodacc/model/StationGroupStationsStation.d \
./pogodacc/model/StationsGroup.d \
./pogodacc/model/Status.d \
./pogodacc/model/Summary.d \
./pogodacc/model/Trend.d \
./pogodacc/model/TrendData.d 

OBJS += \
./pogodacc/model/AvailableParametersModel.o \
./pogodacc/model/LocaleEntry.o \
./pogodacc/model/QualityFactor.o \
./pogodacc/model/StationCurrentLocalTime.o \
./pogodacc/model/StationDataModel.o \
./pogodacc/model/StationDefinitionModel.o \
./pogodacc/model/StationGroupStations.o \
./pogodacc/model/StationGroupStationsStation.o \
./pogodacc/model/StationsGroup.o \
./pogodacc/model/Status.o \
./pogodacc/model/Summary.o \
./pogodacc/model/Trend.o \
./pogodacc/model/TrendData.o 


# Each subdirectory must supply rules for building sources it contributes
pogodacc/model/%.o: ../pogodacc/model/%.cpp pogodacc/model/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-pogodacc-2f-model

clean-pogodacc-2f-model:
	-$(RM) ./pogodacc/model/AvailableParametersModel.d ./pogodacc/model/AvailableParametersModel.o ./pogodacc/model/LocaleEntry.d ./pogodacc/model/LocaleEntry.o ./pogodacc/model/QualityFactor.d ./pogodacc/model/QualityFactor.o ./pogodacc/model/StationCurrentLocalTime.d ./pogodacc/model/StationCurrentLocalTime.o ./pogodacc/model/StationDataModel.d ./pogodacc/model/StationDataModel.o ./pogodacc/model/StationDefinitionModel.d ./pogodacc/model/StationDefinitionModel.o ./pogodacc/model/StationGroupStations.d ./pogodacc/model/StationGroupStations.o ./pogodacc/model/StationGroupStationsStation.d ./pogodacc/model/StationGroupStationsStation.o ./pogodacc/model/StationsGroup.d ./pogodacc/model/StationsGroup.o ./pogodacc/model/Status.d ./pogodacc/model/Status.o ./pogodacc/model/Summary.d ./pogodacc/model/Summary.o ./pogodacc/model/Trend.d ./pogodacc/model/Trend.o ./pogodacc/model/TrendData.d ./pogodacc/model/TrendData.o

.PHONY: clean-pogodacc-2f-model

