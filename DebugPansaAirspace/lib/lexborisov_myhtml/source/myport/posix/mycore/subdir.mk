################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/myport/posix/mycore/io.c \
../lib/lexborisov_myhtml/source/myport/posix/mycore/memory.c \
../lib/lexborisov_myhtml/source/myport/posix/mycore/perf.c \
../lib/lexborisov_myhtml/source/myport/posix/mycore/thread.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/myport/posix/mycore/io.d \
./lib/lexborisov_myhtml/source/myport/posix/mycore/memory.d \
./lib/lexborisov_myhtml/source/myport/posix/mycore/perf.d \
./lib/lexborisov_myhtml/source/myport/posix/mycore/thread.d 

OBJS += \
./lib/lexborisov_myhtml/source/myport/posix/mycore/io.o \
./lib/lexborisov_myhtml/source/myport/posix/mycore/memory.o \
./lib/lexborisov_myhtml/source/myport/posix/mycore/perf.o \
./lib/lexborisov_myhtml/source/myport/posix/mycore/thread.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/myport/posix/mycore/%.o: ../lib/lexborisov_myhtml/source/myport/posix/mycore/%.c lib/lexborisov_myhtml/source/myport/posix/mycore/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -I../lib/kisli_vmime_c6b9ad3c -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore

clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore:
	-$(RM) ./lib/lexborisov_myhtml/source/myport/posix/mycore/io.d ./lib/lexborisov_myhtml/source/myport/posix/mycore/io.o ./lib/lexborisov_myhtml/source/myport/posix/mycore/memory.d ./lib/lexborisov_myhtml/source/myport/posix/mycore/memory.o ./lib/lexborisov_myhtml/source/myport/posix/mycore/perf.d ./lib/lexborisov_myhtml/source/myport/posix/mycore/perf.o ./lib/lexborisov_myhtml/source/myport/posix/mycore/thread.d ./lib/lexborisov_myhtml/source/myport/posix/mycore/thread.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore

