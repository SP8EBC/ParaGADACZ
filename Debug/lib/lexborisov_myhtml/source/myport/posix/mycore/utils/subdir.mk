################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/myport/posix/mycore/utils/mcsync.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/myport/posix/mycore/utils/mcsync.d 

OBJS += \
./lib/lexborisov_myhtml/source/myport/posix/mycore/utils/mcsync.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/myport/posix/mycore/utils/%.o: ../lib/lexborisov_myhtml/source/myport/posix/mycore/utils/%.c lib/lexborisov_myhtml/source/myport/posix/mycore/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -I../lib/kisli_vmime_c6b9ad3c -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore-2f-utils

clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore-2f-utils:
	-$(RM) ./lib/lexborisov_myhtml/source/myport/posix/mycore/utils/mcsync.d ./lib/lexborisov_myhtml/source/myport/posix/mycore/utils/mcsync.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myport-2f-posix-2f-mycore-2f-utils

