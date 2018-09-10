INCLUDES = -I$(top_srcdir) -I$(top_builddir) -I$(top_srcdir)/common
MOSTLYCLEANFILES=
BUILT_SOURCES=

CFLAGS+=$(if $(DEBUG), -g -O0)
CXXFLAGS+=$(if $(DEBUG), -g -O0)

CPPFLAGS+=$(if $(OPTIMIZE), -DNDEBUG)
CFLAGS+=$(if $(OPTIMIZE), -O3)
CXXFLAGS+=$(if $(OPTIMIZE), -O3)

CFLAGS+=$(if $(PROFILE), -pg)
CXXFLAGS+=$(if $(PROFILE), -pg)


#AM_CPPFLAGS=-D'SVNVERSION="$(SVNVERSION)"' -DSS_DOCID_BITS=128
AM_CPPFLAGS=-DSS_DOCID_BITS=128
AM_LDFLAGS=-Wl,-no-undefined
AM_CFLAGS=
AM_CXXFLAGS=

#_SVNVERSION=$(strip $(subst exported, 0, $(shell svnversion -nc $(srcdir) | sed 's/^.*://' | tr -cd [0-9] )))
#SVNVERSION=$(if $(_SVNVERSION),$(_SVNVERSION),0)
CONFIGURE_DEPENDENCIES=$(subst @@@,,$(subst CONFIGURE_DEPENDENCIES,@,@CONFIGURE_DEPENDENCIES@))
CONFIGURE_DEPENDENCIES+=$(top_srcdir)/acsite.m4

#include $(top_srcdir)/collect.mk
