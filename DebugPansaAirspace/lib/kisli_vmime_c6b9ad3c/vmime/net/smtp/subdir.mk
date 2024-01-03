################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/DSNAttributes.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPChunkingOutputStreamAdapter.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommand.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommandSet.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPConnection.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPExceptions.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPResponse.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSTransport.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSendOptions.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPServiceInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPTransport.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/DSNAttributes.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPChunkingOutputStreamAdapter.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommand.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommandSet.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPConnection.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPExceptions.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPResponse.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSTransport.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSendOptions.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPServiceInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPTransport.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/DSNAttributes.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPChunkingOutputStreamAdapter.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommand.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommandSet.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPConnection.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPExceptions.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPResponse.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSTransport.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSendOptions.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPServiceInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPTransport.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-smtp

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-smtp:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/DSNAttributes.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/DSNAttributes.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPChunkingOutputStreamAdapter.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPChunkingOutputStreamAdapter.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommand.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommand.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommandSet.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPCommandSet.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPConnection.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPConnection.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPExceptions.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPExceptions.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPResponse.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPResponse.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSTransport.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSTransport.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSendOptions.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPSendOptions.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPServiceInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPServiceInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPTransport.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/SMTPTransport.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-smtp

