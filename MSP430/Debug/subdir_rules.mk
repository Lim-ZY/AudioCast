################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs2011/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --code_model=small --data_model=small -O4 --opt_for_speed=0 --use_hw_mpy=F5 --include_path="C:/ti/ccs2011/ccs/ccs_base/msp430/include" --include_path="C:/Users/Lim ZY/workspace_ccstheia/AudioCast" --include_path="C:/Users/Lim ZY/workspace_ccstheia/AudioCast/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs2011/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --include_path="C:/Users/Lim ZY/workspace_ccstheia/AudioCast/SI4713" --advice:power="none" --advice:hw_config=all --define=__MSP430FR5969__ --define=_MPU_ENABLE --define=DEPRECATED -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


