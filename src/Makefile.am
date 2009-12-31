bin_PROGRAMS = dhcpmsg
dhcpmsg_SOURCES = dhcpmsg.c
if CHECK
lib_LTLIBRARIES = libutils.la libcurl.la
libutils_la_SOURCES = utils.c
libutils_la_CFLAGS = $(LIBCONFIG)
libcurl_la_SOURCES = curl.c
libcurl_la_CFLAGS = $(LIBCURL)
dhcpmsg_LDADD = libutils.la libcurl.la $(LIBCONFIG) $(LIBCURL)
else
dhcpmsg_SOURCES += utils.c curl.c
dhcpmsg_CFLAGS = $(AM_CFLAGS)
dhcpmsg_LDADD =  $(LIBCONFIG) $(LIBCURL)
endif
