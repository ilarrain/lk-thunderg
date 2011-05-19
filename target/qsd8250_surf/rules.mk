LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/platform/msm_shared

PLATFORM := qsd8k

MEMBASE := 0x00000000 # SMI
MEMSIZE := 0x00100000 # 1MB

BASE_ADDR        := 0x20000000

TAGS_ADDR        := BASE_ADDR+0x00000100
KERNEL_ADDR      := BASE_ADDR+0x00008000
RAMDISK_ADDR     := BASE_ADDR+0x04000000
SCRATCH_ADDR     := BASE_ADDR+0x00001000

KEYS_USE_GPIO_KEYPAD := 1

DEFINES += ENABLE_BATTERY_CHARGING=1

MODULES += \
	dev/keys \
	lib/ptable

DEFINES += \
	SDRAM_SIZE=$(MEMSIZE) \
	BASE_ADDR=$(BASE_ADDR) \
	TAGS_ADDR=$(TAGS_ADDR) \
	KERNEL_ADDR=$(KERNEL_ADDR) \
	RAMDISK_ADDR=$(RAMDISK_ADDR) \
	SCRATCH_ADDR=$(SCRATCH_ADDR)

OBJS += \
	$(LOCAL_DIR)/init.o \
	$(LOCAL_DIR)/atags.o \
	$(LOCAL_DIR)/keypad.o
