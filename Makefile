# Define project name here
PROJECT = EMorpuck

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = e-puck2_main-processor

#Source files to include
CSRC += ./src/main.c \
		./src/motors_thd.c \
		./src/shared_var.c \
		./src/movement.c \
		./src/motors_driver.c \
		./src/infra.c \
		./src/distance.c\
		./src/color.c

#Header folders to include
INCDIR += 

#Jump to the main Makefile
include $(GLOBAL_PATH)/Makefile

clean-libssp:
	@echo Cleaning libssp
	-rm libssp.a libssp_nonshared.a
	@echo
	@echo Done
	


CSRC := $(filter-out $(GLOBAL_PATH)/src/motors.c, $(CSRC))
