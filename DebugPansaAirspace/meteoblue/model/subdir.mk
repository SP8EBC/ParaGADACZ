################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../meteoblue/model/Data_3h.cpp \
../meteoblue/model/Data_day.cpp \
../meteoblue/model/Inline_response_200.cpp \
../meteoblue/model/Metadata.cpp \
../meteoblue/model/Units.cpp 

CPP_DEPS += \
./meteoblue/model/Data_3h.d \
./meteoblue/model/Data_day.d \
./meteoblue/model/Inline_response_200.d \
./meteoblue/model/Metadata.d \
./meteoblue/model/Units.d 

OBJS += \
./meteoblue/model/Data_3h.o \
./meteoblue/model/Data_day.o \
./meteoblue/model/Inline_response_200.o \
./meteoblue/model/Metadata.o \
./meteoblue/model/Units.o 


# Each subdirectory must supply rules for building sources it contributes
meteoblue/model/%.o: ../meteoblue/model/%.cpp meteoblue/model/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-meteoblue-2f-model

clean-meteoblue-2f-model:
	-$(RM) ./meteoblue/model/Data_3h.d ./meteoblue/model/Data_3h.o ./meteoblue/model/Data_day.d ./meteoblue/model/Data_day.o ./meteoblue/model/Inline_response_200.d ./meteoblue/model/Inline_response_200.o ./meteoblue/model/Metadata.d ./meteoblue/model/Metadata.o ./meteoblue/model/Units.d ./meteoblue/model/Units.o

.PHONY: clean-meteoblue-2f-model

