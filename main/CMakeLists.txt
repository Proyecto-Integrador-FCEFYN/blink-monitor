idf_component_register(SRCS "blink.c"
        "monitor.c"
        "procesador_petri.c"
        "manejador_eventos/boton_handler.c"
        "devices/cerradura_device.c"
        "devices/cam.c"
        "manejador_eventos/rfid_handler.c"
        "manejador_eventos/mqtt_handler.c"
        "software/software.c"
        "streaming/streaming.c"
        "devices/comm_dev.c"
        INCLUDE_DIRS
        "."
        "./manejador_eventos"
        "./devices"
        "./streaming"
        "./software")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=missing-braces)
