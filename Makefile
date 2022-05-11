# Define project name here
PROJECT = EMorpuck

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = e-puck2_main-processor

#Source files to include
CSRC += ./src/motion/motors_driver.c \
		./src/motion/movements.c \
		./src/motion/path.c \
		./src/sensors/sensor_color.c \
		./src/sensors/sensor_distance.c\
		./src/sensors/sensor_ir.c \
		./src/scan.c \
		./src/game.c \
		./src/main.c

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

USE_OPT := $(filter-out -O2, $(USE_OPT)) -O0