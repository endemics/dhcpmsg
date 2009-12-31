AC_INIT([dhcpmsg], [0.1], [gildas@endemics.info])
AM_INIT_AUTOMAKE([ -Wall -Werror foreign ])
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])
AC_PROG_LIBTOOL
AC_PROG_CC
AC_CONFIG_HEADERS([ config.h ])
AC_CONFIG_FILES([
                  Makefile
                  src/Makefile
                  tests/Makefile
                  ])
AC_ARG_ENABLE(check,
                  [  --enable-check    Libtool linking to allow check],
                  [case "${enableval}" in
                    yes) check=true ;;
                   no)  check=false ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-check) ;;
                  esac],[check=false])
AM_CONDITIONAL(CHECK, test x$check = xtrue)
AC_ARG_WITH(config,
                  [  --with-config=path Specify the path to config file (default is /etc/dhcpmsg.conf)],
                  [ with_config="$withval" ],
                  [ with_config="/etc/dhcpmsg.conf"])
AC_DEFINE_UNQUOTED([CONFIG_FILE], ["$with_config"], [Path to the config file])
AC_CHECK_LIB(config,
                  [config_init])
AC_CHECK_LIB(curl,
                  [curl_easy_init])
AC_SUBST([LIBCONFIG])
AC_SUBST([LIBCURL])
AC_OUTPUT
