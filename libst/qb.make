libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st' -lpthread -ldl
ifndef debug
  libst.so_NOTSOURCES := debug.c
endif
st.h_SOURCES := st_top.h st_debug.h st_bottom.h
