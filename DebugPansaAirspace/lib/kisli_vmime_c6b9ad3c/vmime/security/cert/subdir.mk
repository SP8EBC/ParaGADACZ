################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/X509Certificate.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateChain.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateExpiredException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateIssuerVerificationException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotTrustedException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotYetValidException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/defaultCertificateVerifier.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/serverIdentityException.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/unsupportedCertificateTypeException.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/X509Certificate.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateChain.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateExpiredException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateIssuerVerificationException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotTrustedException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotYetValidException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/defaultCertificateVerifier.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/serverIdentityException.d \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/unsupportedCertificateTypeException.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/X509Certificate.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateChain.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateExpiredException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateIssuerVerificationException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotTrustedException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotYetValidException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/defaultCertificateVerifier.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/serverIdentityException.o \
./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/unsupportedCertificateTypeException.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/security/cert/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/security/cert/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/security/cert/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-cert

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-cert:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/X509Certificate.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/X509Certificate.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateChain.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateChain.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateExpiredException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateExpiredException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateIssuerVerificationException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateIssuerVerificationException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotTrustedException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotTrustedException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotYetValidException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/certificateNotYetValidException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/defaultCertificateVerifier.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/defaultCertificateVerifier.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/serverIdentityException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/serverIdentityException.o ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/unsupportedCertificateTypeException.d ./lib/kisli_vmime_c6b9ad3c/vmime/security/cert/unsupportedCertificateTypeException.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-security-2f-cert

