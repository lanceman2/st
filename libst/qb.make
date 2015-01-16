libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st' -lpthread -ldl
st.h_SOURCES :=\
 st_top.h\
 st_debug.h\
 st_vector.h\
 st_bottom.h
