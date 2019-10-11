################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ancs_client.c \
../ancs_task.c \
../displayDriver.c \
../displayFonts.c \
../display_task.c \
../largefont.c \
../lis3dh.c \
../lis3dh_task.c \
../main.c \
../miniDB.c \
../smallfont.c \
../storeInFlash_task.c \
../time_client.c \
../touchDriver.c \
../touch_task.c \
../watchAnimations.c 

OBJS += \
./ancs_client.o \
./ancs_task.o \
./displayDriver.o \
./displayFonts.o \
./display_task.o \
./largefont.o \
./lis3dh.o \
./lis3dh_task.o \
./main.o \
./miniDB.o \
./smallfont.o \
./storeInFlash_task.o \
./time_client.o \
./touchDriver.o \
./touch_task.o \
./watchAnimations.o 

C_DEPS += \
./ancs_client.d \
./ancs_task.d \
./displayDriver.d \
./displayFonts.d \
./display_task.d \
./largefont.d \
./lis3dh.d \
./lis3dh_task.d \
./main.d \
./miniDB.d \
./smallfont.d \
./storeInFlash_task.d \
./time_client.d \
./touchDriver.d \
./touch_task.d \
./watchAnimations.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_B -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_B -I"../../../../../sdk/interfaces/ble_stack/" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\adapters\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\config" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\include\adapter" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\include\manager" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\config" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\atts" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\hl\src\rwble_hl" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\em" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llc" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\lld" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llm" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\ll\src\rwble" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ble\profiles" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\ea\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\em\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\hci\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\ip\hci\src" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\common\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\dbg\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\gtl\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\gtl\src" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\h4tl\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\ke\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\ke\src" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\nvds\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\modules\rwip\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\ll\armgcc_4_8" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\boot\armgcc_4_8" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\compiler\armgcc_4_8" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\build\ble-full\reg\fw" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\flash" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\reg" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf\api" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble_services\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble_clients\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\config" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\system\sys_man\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\free_rtos\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\osal" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\peripherals\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\bsp\memory\include" -I"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\projects\dk_apps\ble_profiles\smarchWatch\config" -include"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\projects\dk_apps\ble_profiles\smarchWatch\config\custom_config_qspi.h" -include"E:\DiaSemi\SDK\DA1468x_DA15xxx_SDK_1.0.14.1081\sdk\interfaces\ble\config\ble_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


