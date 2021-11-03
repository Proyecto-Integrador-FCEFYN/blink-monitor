idf_component_register(SRCS "blink.c" "monitor.c" "procesador_petri.c"
                    INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=missing-braces)
