#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_EMBED_TXT_FILES := certs/cacert.pem
COMPONENT_EMBED_TXT_FILES += certs/prvtkey.pem
COMPONENT_EMBED_TXT_FILES += certs/localhost.pem