libstg.so_SOURCES := .
libstg.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^stg'
libstg.so_ADD_PKG := glib-2.0 gsl
libstg.so_LIBADD := ../libst/libst.so
stg.h_SOURCES := stg_top.h stg_debug.h stg_sequence.h stg_bottom.h
