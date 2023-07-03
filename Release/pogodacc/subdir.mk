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

OBJS += \
./pogodacc/ApiClient.o \
./pogodacc/ApiConfiguration.o \
./pogodacc/ApiException.o \
./pogodacc/HttpContent.o \
./pogodacc/JsonBody.o \
./pogodacc/ModelBase.o \
./pogodacc/MultipartFormData.o \
./pogodacc/Object.o 

CPP_DEPS += \
./pogodacc/ApiClient.d \
./pogodacc/ApiConfiguration.d \
./pogodacc/ApiException.d \
./pogodacc/HttpContent.d \
./pogodacc/JsonBody.d \
./pogodacc/ModelBase.d \
./pogodacc/MultipartFormData.d \
./pogodacc/Object.d 


# Each subdirectory must supply rules for building sources it contributes
pogodacc/%.o: ../pogodacc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DNDEBUG -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


