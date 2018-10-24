dnl Setup autoconf
AC_INIT([STFEMTODST], [0.1], [coffe92@gmail.com])

dnl Debug option
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],[enable debugging, defult: no]),
              [case "${enableval}" in
               yes) debug=true  ;;
               no)  debug=false ;;
               *)   AC_MSG_ERROR([bad value ${enablevalue} for --enable-debug]) ;;
             esac], [debug=false])
AC_ARG_WITH(hbtmaker,
            AS_HELP_STRING([--with-hbtmaker],[path to the StHbtMaker source directory, default: ../]),
            [case "${withval}" in
             yes|no) hbtmaker=../ ;;
             *)      hbtmaker=${withval} ;;
           esac], [hbtmaker=../])

dnl Check dependences
echo "Cheking dependences"
AC_LANG(C++)

dnl Disable all CXXFLAGS if it was not declarated
dnl  and check for debug flag
if [test -z $CXXFLAGS; ] then
  if test x"${debug}" == x"true"; then
    CXXFLAGS="-g -O0"
  else
    CXXFLAGS="-O2"
  fi
fi
AC_PROG_CXX

CPPFLAGS="-I${hbtmaker}"

AC_CHECK_PROG(ROOT_CHECK,root,yes)
if test x"$ROOT_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install ROOT before installing.])
fi

dnl Configure Makefile from Makefile.in
AC_OUTPUT(Makefile.gen)
