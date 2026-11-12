# ============================================================================
# Makefile for Windows 10 MS Paint (Native Arch Linux Wayland)
# ============================================================================

BUILD_DIR ?= build

.PHONY: all run clean test install FORCE

all: $(BUILD_DIR)/mspaint

$(BUILD_DIR)/build.ninja: CMakeLists.txt
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release

$(BUILD_DIR)/mspaint: $(BUILD_DIR)/build.ninja FORCE
	cmake --build $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/mspaint

test: all
	./$(BUILD_DIR)/test_canvas
	timeout 2s ./$(BUILD_DIR)/mspaint || [ $$? -eq 124 ]

clean:
	rm -rf $(BUILD_DIR)

install: all
	cmake --install $(BUILD_DIR)
