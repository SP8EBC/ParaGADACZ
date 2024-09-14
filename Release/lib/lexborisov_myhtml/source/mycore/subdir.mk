################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/mycore/incoming.c \
../lib/lexborisov_myhtml/source/mycore/myosi.c \
../lib/lexborisov_myhtml/source/mycore/mystring.c \
../lib/lexborisov_myhtml/source/mycore/mythread.c \
../lib/lexborisov_myhtml/source/mycore/thread_queue.c \
../lib/lexborisov_myhtml/source/mycore/utils.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/mycore/incoming.d \
./lib/lexborisov_myhtml/source/mycore/myosi.d \
./lib/lexborisov_myhtml/source/mycore/mystring.d \
./lib/lexborisov_myhtml/source/mycore/mythread.d \
./lib/lexborisov_myhtml/source/mycore/thread_queue.d \
./lib/lexborisov_myhtml/source/mycore/utils.d 

OBJS += \
./lib/lexborisov_myhtml/source/mycore/incoming.o \
./lib/lexborisov_myhtml/source/mycore/myosi.o \
./lib/lexborisov_myhtml/source/mycore/mystring.o \
./lib/lexborisov_myhtml/source/mycore/mythread.o \
./lib/lexborisov_myhtml/source/mycore/thread_queue.o \
./lib/lexborisov_myhtml/source/mycore/utils.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/mycore/%.o: ../lib/lexborisov_myhtml/source/mycore/%.c lib/lexborisov_myhtml/source/mycore/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -I../lib/kisli_vmime_c6b9ad3c -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore

clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore:
	-$(RM) ./lib/lexborisov_myhtml/source/mycore/incoming.d ./lib/lexborisov_myhtml/source/mycore/incoming.o ./lib/lexborisov_myhtml/source/mycore/myosi.d ./lib/lexborisov_myhtml/source/mycore/myosi.o ./lib/lexborisov_myhtml/source/mycore/mystring.d ./lib/lexborisov_myhtml/source/mycore/mystring.o ./lib/lexborisov_myhtml/source/mycore/mythread.d ./lib/lexborisov_myhtml/source/mycore/mythread.o ./lib/lexborisov_myhtml/source/mycore/thread_queue.d ./lib/lexborisov_myhtml/source/mycore/thread_queue.o ./lib/lexborisov_myhtml/source/mycore/utils.d ./lib/lexborisov_myhtml/source/mycore/utils.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore

