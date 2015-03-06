libst.so_SOURCES := .
libst.so_LDFLAGS := -Wl,-e,_lib_main -export-symbols-regex='^st' -lpthread -ldl
st.h_SOURCES := $(sort $(wildcard st_*.h))

BUILT_SOURCES := deriv_coef.h int_matrix.h

deriv_coef.h: deriv_coef.input diff_coef.input
	open-axiom --script deriv_coef

int_matrix.h: int_matrix.input diff_coef.input
	open-axiom --script int_matrix
