# MeetingCanvas 触控画板项目 Makefile
# 适用于 ARM-CortexA53 开发板

# 编译器设置
CC = arm-linux-gcc
CFLAGS = -Wall -O2 -I./inc
LDFLAGS = -L./lib -lfont -lm

# 源文件
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# 源文件列表
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/main

# 默认目标
all: $(TARGET)

# 创建目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 编译目标
$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "编译完成: $@"

# 编译源文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "清理完成"

# 重新编译
rebuild: clean all

# 安装到开发板
install: $(TARGET)
	@echo "请将 $(TARGET) 拷贝到开发板运行"

# 帮助
help:
	@echo "MeetingCanvas 触控画板项目"
	@echo "使用方法:"
	@echo "  make        - 编译项目"
	@echo "  make clean  - 清理编译文件"
	@echo "  make rebuild- 重新编译"
	@echo "  make install- 安装提示"
	@echo "  make help   - 显示帮助"

.PHONY: all clean rebuild install help