# Information on how to use autotools
# http://www.idryman.org/blog/2016/03/10/autoconf-tutorial-1/
# http://smalltalk.gnu.org/blog/bonzinip/all-you-should-really-know-about-autoconf-and-automake
# http://blog.fourthbit.com/2013/06/18/creating-an-open-source-program-in-c-with-autotools-part-1-of-2

# Must init the autoconf setup
# The first parameter is project name
# second is version number
# third is bug report address
AC_INIT([cell], [1.4.6])

# Safety checks in case user overwritten --srcdir
AC_CONFIG_SRCDIR([cell.c])

# Store the auxiliary build tools (e.g., install-sh, config.sub, config.guess)
# in this dir (build-aux)
AC_CONFIG_AUX_DIR([build-aux])

# Init automake, and specify this program use relaxed structures.
# i.e. this program doesn't follow the gnu coding standards, and doesn't have
# ChangeLog, COPYING, AUTHORS, INSTALL, README etc. files.
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Como usar el coso de dependencias: https://autotools.io/pkgconfig/pkg_check_modules.html
# https://stackoverflow.com/questions/1732822/portably-include-glib-headers-in-autoconf-automake
# parece que libtiff5 se llama "libtiff-4" según pkg-config, tira error poniendo otra cosa
# https://stackoverflow.com/questions/3932742/static-library-auto-discovery-and-linking
# PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.24.1])
PKG_CHECK_MODULES([libtiff], [libtiff-4])
PKG_CHECK_MODULES([libm], [openlibm])

# Check for C compiler
AC_PROG_CC
# We can add more checks in this section

# Tells automake to create a Makefile
# See https://www.gnu.org/software/automake/manual/html_node/Requirements.html
AC_CONFIG_FILES([Makefile])

# Generate the output
AC_OUTPUT
