libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st' -lpthread -ldl
st.h_SOURCES :=\
 st_top.h\
 st_debug.h\
 st_type.h\
 st_sequence.h\
 st_bottom.h

BUILT_SOURCES := sequence_deriv_coef.h sequence_int_matrix.h

sequence_deriv_coef.h: sequence_deriv_coef.input diff_coef.input
	open-axiom --script sequence_deriv_coef

sequence_int_matrix.h: sequence_int_matrix.input diff_coef.input
	open-axiom --script sequence_int_matrix
