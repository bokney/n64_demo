V=1
SOURCE_DIR=.
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

SRC_DIRS := src src/states src/systems

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

OBJS := $(C_FILES:%.c=$(BUILD_DIR)/%.o)

BUILD_DIRS := $(sort $(dir $(OBJS)))

all: hello.z64
.PHONY: all

$(BUILD_DIR)/filesystem.dfs: $(wildcard filesystem/**/*) | $(BUILD_DIR)
	mkdfs $@ filesystem/

hello.z64: N64_ROM_TITLE="Hello World"
hello.z64: $(BUILD_DIR)/filesystem.dfs

$(BUILD_DIR)/hello.elf: $(OBJS)

$(OBJS): | $(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(OBJS:.o=.d)
