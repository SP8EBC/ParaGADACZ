################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pogodacc/model/AvailableParametersModel.cpp \
../pogodacc/model/LocaleEntry.cpp \
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

OBJS += \
./pogodacc/model/AvailableParametersModel.o \
./pogodacc/model/LocaleEntry.o \
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

CPP_DEPS += \
./pogodacc/model/AvailableParametersModel.d \
./pogodacc/model/LocaleEntry.d \
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


# Each subdirectory must supply rules for building sources it contributes
pogodacc/model/%.o: ../pogodacc/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DNDEBUG -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


