V=1
SOURCE_DIR=.
BUILD_DIR=build

include $(N64_INST)/include/n64.mk
include tiny3d/t3d.mk

SRC_DIRS := src src/states src/systems src/gameplay

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS := $(C_FILES:%.c=$(BUILD_DIR)/%.o)
BUILD_DIRS := $(sort $(dir $(OBJS)))

GLB_FILES := $(wildcard assets/models/*.glb)
T3DM_FILES := $(patsubst assets/models/%.glb,filesystem/models/%.t3dm,$(GLB_FILES))

XM_FILES := $(wildcard assets/audio/*.xm)
XM64_FILES := $(patsubst assets/audio/%.xm,filesystem/audio/%.xm64,$(XM_FILES))

PNG_FILES := $(wildcard assets/sprites/*.png)
SPRITE_FILES := $(patsubst assets/sprites/%.png,filesystem/sprites/%.sprite,$(PNG_FILES))

all: hello.z64
.PHONY: all

filesystem/models/%.t3dm: assets/models/%.glb
	@mkdir -p $(dir $@)
	gltf_to_t3d $< $@

filesystem/audio/%.xm64: assets/audio/%.xm
	@mkdir -p $(dir $@)
	audioconv64 -o $(dir $@) $<

filesystem/sprites/%.sprite: assets/sprites/%.png
	@mkdir -p $(dir $@)
	mksprite --format RGBA16 -o $(dir $@) $<

$(BUILD_DIR)/filesystem.dfs: $(T3DM_FILES) $(XM64_FILES) $(SPRITE_FILES) $(wildcard filesystem/**/*) | $(BUILD_DIR)
	find filesystem/ -name ".DS_Store" -delete
	mkdfs $@ filesystem/

hello.z64: N64_ROM_TITLE="Hello World"
hello.z64: $(BUILD_DIR)/filesystem.dfs

$(BUILD_DIR)/hello.elf: $(OBJS)

$(OBJS): | $(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) *.z64 filesystem/models filesystem/audio filesystem/sprites
.PHONY: clean

-include $(OBJS:.o=.d)
