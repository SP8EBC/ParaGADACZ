################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pogodacc/api/ListOfAllStationsApi.cpp \
../pogodacc/api/MeteoSystemApi.cpp \
../pogodacc/api/StationApi.cpp 

CPP_DEPS += \
./pogodacc/api/ListOfAllStationsApi.d \
./pogodacc/api/MeteoSystemApi.d \
./pogodacc/api/StationApi.d 

OBJS += \
./pogodacc/api/ListOfAllStationsApi.o \
./pogodacc/api/MeteoSystemApi.o \
./pogodacc/api/StationApi.o 


# Each subdirectory must supply rules for building sources it contributes
pogodacc/api/%.o: ../pogodacc/api/%.cpp pogodacc/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/karastojko_mailio_0_23_0/include -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-pogodacc-2f-api

clean-pogodacc-2f-api:
	-$(RM) ./pogodacc/api/ListOfAllStationsApi.d ./pogodacc/api/ListOfAllStationsApi.o ./pogodacc/api/MeteoSystemApi.d ./pogodacc/api/MeteoSystemApi.o ./pogodacc/api/StationApi.d ./pogodacc/api/StationApi.o

.PHONY: clean-pogodacc-2f-api

