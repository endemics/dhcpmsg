bin_PROGRAMS = dhcpmsg
dhcpmsg_SOURCES = dhcpmsg.c
if CHECK
lib_LTLIBRARIES = libutils.la
libutils_la_SOURCES = utils.c
dhcpmsg_LDADD = libutils.la
else
dhcpmsg_SOURCES += utils.c
dhcpmsg_CFLAGS = $(AM_CFLAGS)
endif
