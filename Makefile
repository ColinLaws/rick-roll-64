BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = main.c

all: RickRoll64.z64

$(BUILD_DIR)/RickRoll64.dfs: filesystem/*
$(BUILD_DIR)/RickRoll64.elf: $(src:%.c=$(BUILD_DIR)/%.o)

RickRoll64.z64: N64_ROM_TITLE="Video Player"
RickRoll64.z64: $(BUILD_DIR)/RickRoll64.dfs

clean:
	rm -rf $(BUILD_DIR) RickRoll64.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
