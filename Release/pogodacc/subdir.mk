################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pogodacc/ApiClient.cpp \
../pogodacc/ApiConfiguration.cpp \
../pogodacc/ApiException.cpp \
../pogodacc/HttpContent.cpp \
../pogodacc/JsonBody.cpp \
../pogodacc/ModelBase.cpp \
../pogodacc/MultipartFormData.cpp \
../pogodacc/Object.cpp 

CPP_DEPS += \
./pogodacc/ApiClient.d \
./pogodacc/ApiConfiguration.d \
./pogodacc/ApiException.d \
./pogodacc/HttpContent.d \
./pogodacc/JsonBody.d \
./pogodacc/ModelBase.d \
./pogodacc/MultipartFormData.d \
./pogodacc/Object.d 

OBJS += \
./pogodacc/ApiClient.o \
./pogodacc/ApiConfiguration.o \
./pogodacc/ApiException.o \
./pogodacc/HttpContent.o \
./pogodacc/JsonBody.o \
./pogodacc/ModelBase.o \
./pogodacc/MultipartFormData.o \
./pogodacc/Object.o 


# Each subdirectory must supply rules for building sources it contributes
pogodacc/%.o: ../pogodacc/%.cpp pogodacc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-pogodacc

clean-pogodacc:
	-$(RM) ./pogodacc/ApiClient.d ./pogodacc/ApiClient.o ./pogodacc/ApiConfiguration.d ./pogodacc/ApiConfiguration.o ./pogodacc/ApiException.d ./pogodacc/ApiException.o ./pogodacc/HttpContent.d ./pogodacc/HttpContent.o ./pogodacc/JsonBody.d ./pogodacc/JsonBody.o ./pogodacc/ModelBase.d ./pogodacc/ModelBase.o ./pogodacc/MultipartFormData.d ./pogodacc/MultipartFormData.o ./pogodacc/Object.d ./pogodacc/Object.o

.PHONY: clean-pogodacc

