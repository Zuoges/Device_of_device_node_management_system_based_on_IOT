CSRCS += lv_qrcode.c
CSRCS += qrcodegen.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_qrc
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_qrc

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_qrc"
