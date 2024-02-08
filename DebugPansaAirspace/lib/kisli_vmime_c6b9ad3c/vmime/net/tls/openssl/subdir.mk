################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/OpenSSLInitializer.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSProperties_OpenSSL.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSession_OpenSSL.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSocket_OpenSSL.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/OpenSSLInitializer.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSProperties_OpenSSL.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSession_OpenSSL.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSocket_OpenSSL.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/OpenSSLInitializer.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSProperties_OpenSSL.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSession_OpenSSL.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSocket_OpenSSL.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls-2f-openssl

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls-2f-openssl:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/OpenSSLInitializer.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/OpenSSLInitializer.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSProperties_OpenSSL.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSProperties_OpenSSL.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSession_OpenSSL.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSession_OpenSSL.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSocket_OpenSSL.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/TLSSocket_OpenSSL.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-tls-2f-openssl

