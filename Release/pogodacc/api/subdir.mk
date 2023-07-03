################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pogodacc/api/ListOfAllStationsApi.cpp \
../pogodacc/api/MeteoSystemApi.cpp \
../pogodacc/api/StationApi.cpp 

OBJS += \
./pogodacc/api/ListOfAllStationsApi.o \
./pogodacc/api/MeteoSystemApi.o \
./pogodacc/api/StationApi.o 

CPP_DEPS += \
./pogodacc/api/ListOfAllStationsApi.d \
./pogodacc/api/MeteoSystemApi.d \
./pogodacc/api/StationApi.d 


# Each subdirectory must supply rules for building sources it contributes
pogodacc/api/%.o: ../pogodacc/api/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DNDEBUG -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


