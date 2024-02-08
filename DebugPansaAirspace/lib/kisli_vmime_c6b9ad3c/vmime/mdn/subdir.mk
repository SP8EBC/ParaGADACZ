################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNHelper.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/mdn/receivedMDNInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/mdn/sendableMDNInfos.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNHelper.d \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/receivedMDNInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/sendableMDNInfos.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNHelper.o \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/receivedMDNInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/mdn/sendableMDNInfos.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/mdn/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/mdn/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/mdn/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-mdn

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-mdn:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNHelper.d ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNHelper.o ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/MDNInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/receivedMDNInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/receivedMDNInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/sendableMDNInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/mdn/sendableMDNInfos.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-mdn

