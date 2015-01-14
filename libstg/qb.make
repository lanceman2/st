libstg.so_SOURCES := .
libstg.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^stg' -lpthread -ldl
ifndef debug
  libstg.so_NOTSOURCES := debug.c
endif
libstg.so_ADD_PKG := glib-2.0
