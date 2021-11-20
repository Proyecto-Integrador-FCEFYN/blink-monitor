idf_component_register(SRCS "blink.c"
        "monitor.c"
        "procesador_petri.c"
        "devices/boton_device.c"
        "./devices/device.c"
        "./devices/cerradura_device.c"
        "./devices/cam.c"
        "./devices/mqtt.c"
        INCLUDE_DIRS
        "."
        "./devices")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=missing-braces)
