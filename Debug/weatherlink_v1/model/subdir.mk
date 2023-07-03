################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../weatherlink_v1/model/Davis_current_observation.cpp \
../weatherlink_v1/model/Image.cpp \
../weatherlink_v1/model/Root.cpp 

CPP_DEPS += \
./weatherlink_v1/model/Davis_current_observation.d \
./weatherlink_v1/model/Image.d \
./weatherlink_v1/model/Root.d 

OBJS += \
./weatherlink_v1/model/Davis_current_observation.o \
./weatherlink_v1/model/Image.o \
./weatherlink_v1/model/Root.o 


# Each subdirectory must supply rules for building sources it contributes
weatherlink_v1/model/%.o: ../weatherlink_v1/model/%.cpp weatherlink_v1/model/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-weatherlink_v1-2f-model

clean-weatherlink_v1-2f-model:
	-$(RM) ./weatherlink_v1/model/Davis_current_observation.d ./weatherlink_v1/model/Davis_current_observation.o ./weatherlink_v1/model/Image.d ./weatherlink_v1/model/Image.o ./weatherlink_v1/model/Root.d ./weatherlink_v1/model/Root.o

.PHONY: clean-weatherlink_v1-2f-model

