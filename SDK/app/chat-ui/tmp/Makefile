CC = /home/xiao/project/buildroot-2021.02.4/output/host/bin/arm-buildroot-linux-gnueabi-gcc
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= $(shell pwd)
CFLAGS ?= -O0 -g -I$(LVGL_DIR)/ -I$(LVGL_DIR)/lv_port_linux_frame_buffer/lvgl -I$(LVGL_DIR)/lv_port_linux_frame_buffer/lv_drivers -I$(LVGL_DIR)/ui_app
LDFLAGS ?= -lm
BIN = demo

MAINSRC = ./main.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
include $(LVGL_DIR)/ui_app/app.mk

OBJEXT ?= .o
OBJDIR = output

# 保留源文件路径结构的目标文件定义
COBJS = $(addprefix $(OBJDIR)/, $(CSRCS:.c=.o))
AOBJS = $(addprefix $(OBJDIR)/, $(ASRCS:.S=.o))
MAINOBJ = $(addprefix $(OBJDIR)/, $(MAINSRC:.c=.o))

OBJS = $(AOBJS) $(COBJS)

# 定义输出目录结构
OUTPUT_DIR = output
BUILD_DIR = $(OUTPUT_DIR)/build

# 生成目标文件路径，保持源文件的目录结构
define generate_object_files
$(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(1)))
endef

# 将所有对象文件放在 output 目录下，保持源文件的目录结构
AOBJS = $(addprefix $(BUILD_DIR)/, $(patsubst %.S,%.o,$(ASRCS)))
COBJS = $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(CSRCS)))
MAINOBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(MAINSRC)))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

# 编译规则
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC $<"

all: default

default: $(BUILD_DIR) $(OBJS) $(MAINOBJ)
	@mkdir -p $(OUTPUT_DIR)
	$(CC) -o $(OUTPUT_DIR)/$(BIN) $(MAINOBJ) $(OBJS) $(LDFLAGS)
	@echo "Build complete: $(OUTPUT_DIR)/$(BIN)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean: 
	rm -f $(OUTPUT_DIR)/$(BIN)
	rm -rf $(BUILD_DIR)