################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Command.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Connection.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Folder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3FolderStatus.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Message.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Response.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3SStore.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3ServiceInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Store.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Utils.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Command.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Connection.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Folder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3FolderStatus.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Message.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Response.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3SStore.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3ServiceInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Store.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Utils.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Command.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Connection.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Folder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3FolderStatus.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Message.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Response.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3SStore.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3ServiceInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Store.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Utils.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-pop3

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-pop3:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Command.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Command.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Connection.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Connection.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Folder.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Folder.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3FolderStatus.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3FolderStatus.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Message.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Message.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Response.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Response.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3SStore.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3SStore.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3ServiceInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3ServiceInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Store.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Store.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Utils.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/POP3Utils.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-pop3

