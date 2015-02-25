DIST_CLEAN := if [ -f examples/GNUmakefile ] ; then $(MAKE) -C examples/ distclean ; fi
SUBDIRS    := libst libstg bin tests doc
