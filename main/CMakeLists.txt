idf_component_register(SRCS "blink.c"
        "monitor.c"
        "procesador_petri.c"
        "./devices/gpio_device.c"
        "./devices/device.c"
        INCLUDE_DIRS
        "."
        "./devices")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=missing-braces)
