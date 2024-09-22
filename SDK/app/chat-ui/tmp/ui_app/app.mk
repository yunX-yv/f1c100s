APP_DIR_NAME ?= ui_app

CSRCS += $(wildcard $(LVGL_DIR)/$(APP_DIR_NAME)/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(APP_DIR_NAME)/components/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(APP_DIR_NAME)/fonts/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(APP_DIR_NAME)/images/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(APP_DIR_NAME)/screens/*.c)
