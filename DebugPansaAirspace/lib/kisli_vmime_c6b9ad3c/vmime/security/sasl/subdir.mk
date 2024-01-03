################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLContext.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLMechanismFactory.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSession.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSocket.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLAuthenticator.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLMechanism.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/builtinSASLMechanism.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/defaultSASLAuthenticator.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLContext.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLMechanismFactory.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSession.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSocket.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLAuthenticator.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLMechanism.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/builtinSASLMechanism.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/defaultSASLAuthenticator.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLContext.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLMechanismFactory.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSession.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSocket.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLAuthenticator.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLMechanism.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/builtinSASLMechanism.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/defaultSASLAuthenticator.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-sasl

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-sasl:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLContext.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLContext.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLMechanismFactory.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLMechanismFactory.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSession.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSession.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSocket.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/SASLSocket.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLAuthenticator.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLAuthenticator.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLMechanism.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/XOAuth2SASLMechanism.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/builtinSASLMechanism.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/builtinSASLMechanism.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/defaultSASLAuthenticator.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/defaultSASLAuthenticator.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-sasl

