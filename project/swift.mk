# top level project rules for the LG GT540 (swift) project
#
LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := swift

MODULES += app/aboot

#DEFINES += WITH_DEBUG_DCC=1
#DEFINES += WITH_DEBUG_UART=1
DEFINES += WITH_DEBUG_FBCON=1
