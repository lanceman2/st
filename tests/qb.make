# install nothing from this dir
NO_INSTALL := .
# build a binary from every .c file
#BINS := .  it's the default
debug_LIBADD := ../libst/libst.so ../libstg/libstg.so
debug_ADD_PKG := glib-2.0
stSequence_print_LIBADD := ../libst/libst.so
stSequence_printFunc_LIBADD := ../libst/libst.so
stSequence_deriv_LIBADD := ../libst/libst.so
stSequence_deriv_ADD_PKG := gsl
