TARGET_EXEC := week05
TARGET_TEST_EXEC := test_runner

BUILD_DIR := build
SRC_DIR := src
TEST_SRC_DIR := test

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_SRCS := $(shell find $(TEST_SRC_DIR) -name 'test-*.cpp')
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	clang++ -g $(OBJS) -o $@

$(BUILD_DIR)/$(TARGET_TEST_EXEC): $(TEST_OBJS)
	clang++ -g $(TEST_OBJS) -o $@
	$(BUILD_DIR)/$(TARGET_TEST_EXEC)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	clang++ -std=c++11 -c $< -o $@

all : $(BUILD_DIR)/$(TARGET_EXEC)

test: $(BUILD_DIR)/$(TARGET_TEST_EXEC)

.PHONY: clean
clean :
	rm -rf build/*