libstg.so_SOURCES := .
libstg.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^stg' -lpthread -ldl
libstg.so_ADD_PKG := glib-2.0
stg.h_SOURCES := stg_top.h stg_debug.h stg_bottom.h
