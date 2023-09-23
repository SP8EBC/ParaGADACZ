################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixChildProcess.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixCriticalSection.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixFile.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixHandler.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixSocket.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixChildProcess.d \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixCriticalSection.d \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixFile.d \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixHandler.d \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixSocket.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixChildProcess.o \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixCriticalSection.o \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixFile.o \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixHandler.o \
./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixSocket.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-platforms-2f-posix

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-platforms-2f-posix:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixChildProcess.d ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixChildProcess.o ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixCriticalSection.d ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixCriticalSection.o ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixFile.d ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixFile.o ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixHandler.d ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixHandler.o ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixSocket.d ./lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/posixSocket.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-platforms-2f-posix

