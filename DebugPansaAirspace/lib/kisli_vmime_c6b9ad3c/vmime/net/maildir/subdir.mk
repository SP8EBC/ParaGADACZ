################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolder.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolderStatus.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFormat.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessage.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessagePart.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessageStructure.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirServiceInfos.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirStore.cpp \
../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirUtils.cpp 

CPP_DEPS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolder.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolderStatus.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFormat.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessage.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessagePart.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessageStructure.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirServiceInfos.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirStore.d \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirUtils.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolder.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolderStatus.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFormat.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessage.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessagePart.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessageStructure.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirServiceInfos.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirStore.o \
./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirUtils.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/%.o: ../lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/%.cpp lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE_ENABLED -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-maildir

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-maildir:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolder.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolder.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolderStatus.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFolderStatus.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFormat.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirFormat.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessage.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessage.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessagePart.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessagePart.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessageStructure.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirMessageStructure.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirServiceInfos.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirServiceInfos.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirStore.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirStore.o ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirUtils.d ./lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/maildirUtils.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-vmime-2f-net-2f-maildir

