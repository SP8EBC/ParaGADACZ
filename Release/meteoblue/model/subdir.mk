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

OBJS += \
./meteoblue/model/Data_3h.o \
./meteoblue/model/Data_day.o \
./meteoblue/model/Inline_response_200.o \
./meteoblue/model/Metadata.o \
./meteoblue/model/Units.o 

CPP_DEPS += \
./meteoblue/model/Data_3h.d \
./meteoblue/model/Data_day.d \
./meteoblue/model/Inline_response_200.d \
./meteoblue/model/Metadata.d \
./meteoblue/model/Units.d 


# Each subdirectory must supply rules for building sources it contributes
meteoblue/model/%.o: ../meteoblue/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DNDEBUG -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO -I../lib/nlohman_json_3_7_1/include/ -I../meteoblue/api -I../meteoblue/model -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


