# cellID-linux
This is meant to aid compilation of cellID v1.4 in GNU/linux systems

# Dependencies
Disclaimer: it is possible that not all of the installed packages are strictly required. I have not yet checked, but surely the "-dev" ones are essential.

I have made use of autotools, and have installed the following packages:
  automake autoconf libtool

The configure.ac and Makefile.am files have been set to require libtiff5-dev, libopenlibm-dev, and libglib2.0-dev.

You may install these in Ubuntu systems by running:
    sudo apt-get install libglib2.0 libglib2.0-dev libtiff5 libtiff5-dev libopenlibm-dev
    
You may find details on how the automake/conf files are configured by opening them and reading the comments.
I must say that although packages are installed using one name (such as libtiff5) the name that configure.ac can find may be another one (such as libtiff-4).

# Build and Install

To build and install, please cd into the directory where the files are and run:
  autoreconf -fvi
  ./configure
  make -j8
  sudo make install

Please refer to the cellID documentation to learn how to use this program.

Cheers!
