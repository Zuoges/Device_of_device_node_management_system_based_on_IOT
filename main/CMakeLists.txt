set(COMPONENT_SRCDIRS   src  
                        src/hardware
                        src/IM1281B
                        src/wifi
                        src/mqtt
                        src/lvgl
                        src/lvgl/img
                        src/lvgl/font
                        src/event
                        src/serial_bus
                        src/server_communicate
                        )
set(COMPONENT_ADD_INCLUDEDIRS 
                        inc 
                        inc/hardware
                        inc/IM1281B
                        inc/wifi
                        inc/mqtt
                        inc/lvgl
                        inc/event
                        inc/serial_bus
                        inc/server_communicate
                        )

register_component()