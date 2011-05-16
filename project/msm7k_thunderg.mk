# top level project rules for the msm7k_thunderg project
#
LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := msm7k_thunderg

MODULES += app/aboot

#DEFINES += WITH_DEBUG_DCC=1
#DEFINES += WITH_DEBUG_UART=1
DEFINES += WITH_DEBUG_FBCON=1
