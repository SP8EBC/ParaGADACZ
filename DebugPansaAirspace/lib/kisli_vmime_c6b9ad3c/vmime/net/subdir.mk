################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/defaultConnectionInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/defaultTimeoutHandler.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/events.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/fetchAttributes.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/folder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/folderAttributes.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/message.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/messageSet.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/service.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/serviceFactory.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/serviceInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/session.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/store.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/tracer.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/transport.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultConnectionInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultTimeoutHandler.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/events.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/fetchAttributes.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/folder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/folderAttributes.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/message.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/messageSet.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/service.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceFactory.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/session.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/store.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tracer.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/transport.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultConnectionInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultTimeoutHandler.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/events.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/fetchAttributes.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/folder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/folderAttributes.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/message.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/messageSet.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/service.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceFactory.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/session.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/store.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/tracer.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/transport.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultConnectionInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultConnectionInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultTimeoutHandler.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/defaultTimeoutHandler.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/events.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/events.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/fetchAttributes.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/fetchAttributes.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/folder.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/folder.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/folderAttributes.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/folderAttributes.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/message.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/message.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/messageSet.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/messageSet.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/service.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/service.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceFactory.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceFactory.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/serviceInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/session.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/session.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/store.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/store.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/tracer.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/tracer.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/transport.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/transport.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net

