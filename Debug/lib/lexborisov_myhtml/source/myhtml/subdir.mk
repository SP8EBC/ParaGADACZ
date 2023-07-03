################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/myhtml/callback.c \
../lib/lexborisov_myhtml/source/myhtml/charef.c \
../lib/lexborisov_myhtml/source/myhtml/data_process.c \
../lib/lexborisov_myhtml/source/myhtml/myhtml.c \
../lib/lexborisov_myhtml/source/myhtml/mynamespace.c \
../lib/lexborisov_myhtml/source/myhtml/mystring.c \
../lib/lexborisov_myhtml/source/myhtml/parser.c \
../lib/lexborisov_myhtml/source/myhtml/rules.c \
../lib/lexborisov_myhtml/source/myhtml/serialization.c \
../lib/lexborisov_myhtml/source/myhtml/stream.c \
../lib/lexborisov_myhtml/source/myhtml/tag.c \
../lib/lexborisov_myhtml/source/myhtml/tag_init.c \
../lib/lexborisov_myhtml/source/myhtml/token.c \
../lib/lexborisov_myhtml/source/myhtml/tokenizer.c \
../lib/lexborisov_myhtml/source/myhtml/tokenizer_doctype.c \
../lib/lexborisov_myhtml/source/myhtml/tokenizer_end.c \
../lib/lexborisov_myhtml/source/myhtml/tokenizer_script.c \
../lib/lexborisov_myhtml/source/myhtml/tree.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/myhtml/callback.d \
./lib/lexborisov_myhtml/source/myhtml/charef.d \
./lib/lexborisov_myhtml/source/myhtml/data_process.d \
./lib/lexborisov_myhtml/source/myhtml/myhtml.d \
./lib/lexborisov_myhtml/source/myhtml/mynamespace.d \
./lib/lexborisov_myhtml/source/myhtml/mystring.d \
./lib/lexborisov_myhtml/source/myhtml/parser.d \
./lib/lexborisov_myhtml/source/myhtml/rules.d \
./lib/lexborisov_myhtml/source/myhtml/serialization.d \
./lib/lexborisov_myhtml/source/myhtml/stream.d \
./lib/lexborisov_myhtml/source/myhtml/tag.d \
./lib/lexborisov_myhtml/source/myhtml/tag_init.d \
./lib/lexborisov_myhtml/source/myhtml/token.d \
./lib/lexborisov_myhtml/source/myhtml/tokenizer.d \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_doctype.d \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_end.d \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_script.d \
./lib/lexborisov_myhtml/source/myhtml/tree.d 

OBJS += \
./lib/lexborisov_myhtml/source/myhtml/callback.o \
./lib/lexborisov_myhtml/source/myhtml/charef.o \
./lib/lexborisov_myhtml/source/myhtml/data_process.o \
./lib/lexborisov_myhtml/source/myhtml/myhtml.o \
./lib/lexborisov_myhtml/source/myhtml/mynamespace.o \
./lib/lexborisov_myhtml/source/myhtml/mystring.o \
./lib/lexborisov_myhtml/source/myhtml/parser.o \
./lib/lexborisov_myhtml/source/myhtml/rules.o \
./lib/lexborisov_myhtml/source/myhtml/serialization.o \
./lib/lexborisov_myhtml/source/myhtml/stream.o \
./lib/lexborisov_myhtml/source/myhtml/tag.o \
./lib/lexborisov_myhtml/source/myhtml/tag_init.o \
./lib/lexborisov_myhtml/source/myhtml/token.o \
./lib/lexborisov_myhtml/source/myhtml/tokenizer.o \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_doctype.o \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_end.o \
./lib/lexborisov_myhtml/source/myhtml/tokenizer_script.o \
./lib/lexborisov_myhtml/source/myhtml/tree.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/myhtml/%.o: ../lib/lexborisov_myhtml/source/myhtml/%.c lib/lexborisov_myhtml/source/myhtml/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myhtml

clean-lib-2f-lexborisov_myhtml-2f-source-2f-myhtml:
	-$(RM) ./lib/lexborisov_myhtml/source/myhtml/callback.d ./lib/lexborisov_myhtml/source/myhtml/callback.o ./lib/lexborisov_myhtml/source/myhtml/charef.d ./lib/lexborisov_myhtml/source/myhtml/charef.o ./lib/lexborisov_myhtml/source/myhtml/data_process.d ./lib/lexborisov_myhtml/source/myhtml/data_process.o ./lib/lexborisov_myhtml/source/myhtml/myhtml.d ./lib/lexborisov_myhtml/source/myhtml/myhtml.o ./lib/lexborisov_myhtml/source/myhtml/mynamespace.d ./lib/lexborisov_myhtml/source/myhtml/mynamespace.o ./lib/lexborisov_myhtml/source/myhtml/mystring.d ./lib/lexborisov_myhtml/source/myhtml/mystring.o ./lib/lexborisov_myhtml/source/myhtml/parser.d ./lib/lexborisov_myhtml/source/myhtml/parser.o ./lib/lexborisov_myhtml/source/myhtml/rules.d ./lib/lexborisov_myhtml/source/myhtml/rules.o ./lib/lexborisov_myhtml/source/myhtml/serialization.d ./lib/lexborisov_myhtml/source/myhtml/serialization.o ./lib/lexborisov_myhtml/source/myhtml/stream.d ./lib/lexborisov_myhtml/source/myhtml/stream.o ./lib/lexborisov_myhtml/source/myhtml/tag.d ./lib/lexborisov_myhtml/source/myhtml/tag.o ./lib/lexborisov_myhtml/source/myhtml/tag_init.d ./lib/lexborisov_myhtml/source/myhtml/tag_init.o ./lib/lexborisov_myhtml/source/myhtml/token.d ./lib/lexborisov_myhtml/source/myhtml/token.o ./lib/lexborisov_myhtml/source/myhtml/tokenizer.d ./lib/lexborisov_myhtml/source/myhtml/tokenizer.o ./lib/lexborisov_myhtml/source/myhtml/tokenizer_doctype.d ./lib/lexborisov_myhtml/source/myhtml/tokenizer_doctype.o ./lib/lexborisov_myhtml/source/myhtml/tokenizer_end.d ./lib/lexborisov_myhtml/source/myhtml/tokenizer_end.o ./lib/lexborisov_myhtml/source/myhtml/tokenizer_script.d ./lib/lexborisov_myhtml/source/myhtml/tokenizer_script.o ./lib/lexborisov_myhtml/source/myhtml/tree.d ./lib/lexborisov_myhtml/source/myhtml/tree.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myhtml

