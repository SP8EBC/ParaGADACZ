################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../meteoblue/api/ForecastApi.cpp 

CPP_DEPS += \
./meteoblue/api/ForecastApi.d 

OBJS += \
./meteoblue/api/ForecastApi.o 


# Each subdirectory must supply rules for building sources it contributes
meteoblue/api/%.o: ../meteoblue/api/%.cpp meteoblue/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-meteoblue-2f-api

clean-meteoblue-2f-api:
	-$(RM) ./meteoblue/api/ForecastApi.d ./meteoblue/api/ForecastApi.o

.PHONY: clean-meteoblue-2f-api

