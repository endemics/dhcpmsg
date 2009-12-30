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
AC_OUTPUT
