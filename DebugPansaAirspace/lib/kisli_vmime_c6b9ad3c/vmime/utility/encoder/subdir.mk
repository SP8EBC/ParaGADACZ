################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/b64Encoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/binaryEncoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/eightBitEncoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoderFactory.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/noopEncoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/qpEncoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/sevenBitEncoder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/uuEncoder.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/b64Encoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/binaryEncoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/eightBitEncoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoderFactory.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/noopEncoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/qpEncoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/sevenBitEncoder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/uuEncoder.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/b64Encoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/binaryEncoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/eightBitEncoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoderFactory.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/noopEncoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/qpEncoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/sevenBitEncoder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/uuEncoder.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-utility-2f-encoder

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-utility-2f-encoder:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/b64Encoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/b64Encoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/binaryEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/binaryEncoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/eightBitEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/eightBitEncoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoderFactory.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/encoderFactory.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/noopEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/noopEncoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/qpEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/qpEncoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/sevenBitEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/sevenBitEncoder.o ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/uuEncoder.d ./lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/uuEncoder.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-utility-2f-encoder

