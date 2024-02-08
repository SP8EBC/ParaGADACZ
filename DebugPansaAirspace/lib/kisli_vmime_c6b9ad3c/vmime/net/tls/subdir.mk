################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSProperties.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSecuredConnectionInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSession.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSocket.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSProperties.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSecuredConnectionInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSession.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSocket.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSProperties.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSecuredConnectionInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSession.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSocket.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/tls/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/tls/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSProperties.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSProperties.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSecuredConnectionInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSecuredConnectionInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSession.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSession.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSocket.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/TLSSocket.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls

