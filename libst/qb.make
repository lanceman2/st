libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st'
