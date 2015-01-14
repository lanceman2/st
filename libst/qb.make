libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st' -lpthread -ldl
ifndef debug
  libst.so_NOTSOURCES := debug.c
endif
