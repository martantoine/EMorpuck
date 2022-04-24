# Define project name here
PROJECT = EMorpuck

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = e-puck2_main-processor

#Source files to include
CSRC += ./src/main.c \
		./src/motors_thd.c \
#		./src/motors_pos.c \
		./shared_var.c \
		./movement.c \

#Header folders to include
INCDIR += 

#Jump to the main Makefile
include $(GLOBAL_PATH)/Makefile