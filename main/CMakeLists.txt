idf_component_register(SRCS "blink.c"
        "monitor.c"
        "procesador_petri.c"
        "manejador_eventos/boton_handler.c"
        "manejador_eventos/movimiento_handler.c"
        "devices/cerradura_device.c"
        "devices/cam.c"
        "manejador_eventos/rfid_handler.c"
        "software/software.c"
        "streaming/streaming.c"
        INCLUDE_DIRS
        "."
        "./manejador_eventos"
        "./devices"
        "./streaming"
        "./software"
        EMBED_TXTFILES
        "certs/cacert.pem"
        "certs/prvtkey.pem"
        "certs/localhost.pem"
        )

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=missing-braces)
