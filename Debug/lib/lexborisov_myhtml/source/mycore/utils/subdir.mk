################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/mycore/utils/avl_tree.c \
../lib/lexborisov_myhtml/source/mycore/utils/mchar_async.c \
../lib/lexborisov_myhtml/source/mycore/utils/mcobject.c \
../lib/lexborisov_myhtml/source/mycore/utils/mcobject_async.c \
../lib/lexborisov_myhtml/source/mycore/utils/mcsimple.c \
../lib/lexborisov_myhtml/source/mycore/utils/mcsync.c \
../lib/lexborisov_myhtml/source/mycore/utils/mctree.c \
../lib/lexborisov_myhtml/source/mycore/utils/mhash.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/mycore/utils/avl_tree.d \
./lib/lexborisov_myhtml/source/mycore/utils/mchar_async.d \
./lib/lexborisov_myhtml/source/mycore/utils/mcobject.d \
./lib/lexborisov_myhtml/source/mycore/utils/mcobject_async.d \
./lib/lexborisov_myhtml/source/mycore/utils/mcsimple.d \
./lib/lexborisov_myhtml/source/mycore/utils/mcsync.d \
./lib/lexborisov_myhtml/source/mycore/utils/mctree.d \
./lib/lexborisov_myhtml/source/mycore/utils/mhash.d 

OBJS += \
./lib/lexborisov_myhtml/source/mycore/utils/avl_tree.o \
./lib/lexborisov_myhtml/source/mycore/utils/mchar_async.o \
./lib/lexborisov_myhtml/source/mycore/utils/mcobject.o \
./lib/lexborisov_myhtml/source/mycore/utils/mcobject_async.o \
./lib/lexborisov_myhtml/source/mycore/utils/mcsimple.o \
./lib/lexborisov_myhtml/source/mycore/utils/mcsync.o \
./lib/lexborisov_myhtml/source/mycore/utils/mctree.o \
./lib/lexborisov_myhtml/source/mycore/utils/mhash.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/mycore/utils/%.o: ../lib/lexborisov_myhtml/source/mycore/utils/%.c lib/lexborisov_myhtml/source/mycore/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore-2f-utils

clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore-2f-utils:
	-$(RM) ./lib/lexborisov_myhtml/source/mycore/utils/avl_tree.d ./lib/lexborisov_myhtml/source/mycore/utils/avl_tree.o ./lib/lexborisov_myhtml/source/mycore/utils/mchar_async.d ./lib/lexborisov_myhtml/source/mycore/utils/mchar_async.o ./lib/lexborisov_myhtml/source/mycore/utils/mcobject.d ./lib/lexborisov_myhtml/source/mycore/utils/mcobject.o ./lib/lexborisov_myhtml/source/mycore/utils/mcobject_async.d ./lib/lexborisov_myhtml/source/mycore/utils/mcobject_async.o ./lib/lexborisov_myhtml/source/mycore/utils/mcsimple.d ./lib/lexborisov_myhtml/source/mycore/utils/mcsimple.o ./lib/lexborisov_myhtml/source/mycore/utils/mcsync.d ./lib/lexborisov_myhtml/source/mycore/utils/mcsync.o ./lib/lexborisov_myhtml/source/mycore/utils/mctree.d ./lib/lexborisov_myhtml/source/mycore/utils/mctree.o ./lib/lexborisov_myhtml/source/mycore/utils/mhash.d ./lib/lexborisov_myhtml/source/mycore/utils/mhash.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-mycore-2f-utils

