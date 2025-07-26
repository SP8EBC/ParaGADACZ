################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPCommand.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPConnection.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolderStatus.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessage.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePart.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePartContentHandler.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessageStructure.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSStore.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSearchAttributes.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPServiceInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPStore.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPTag.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPUtils.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPCommand.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPConnection.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolderStatus.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessage.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePart.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePartContentHandler.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessageStructure.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSStore.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSearchAttributes.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPServiceInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPStore.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPTag.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPUtils.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPCommand.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPConnection.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolderStatus.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessage.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePart.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePartContentHandler.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessageStructure.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSStore.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSearchAttributes.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPServiceInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPStore.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPTag.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPUtils.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/imap/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/imap/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/imap/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-imap

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-imap:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPCommand.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPCommand.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPConnection.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPConnection.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolder.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolder.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolderStatus.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPFolderStatus.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessage.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessage.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePart.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePart.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePartContentHandler.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessagePartContentHandler.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessageStructure.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPMessageStructure.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSStore.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSStore.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSearchAttributes.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPSearchAttributes.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPServiceInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPServiceInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPStore.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPStore.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPTag.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPTag.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPUtils.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/imap/IMAPUtils.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-imap

