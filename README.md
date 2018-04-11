# cellID-linux
This is meant to aid compilation of cellID v1.4 in GNU/linux systems

I became tired of fighting with unincluded dependencies, and so tried autotools by following [this](https://robots.thoughtbot.com/the-magic-behind-configure-make-make-install) tutorial and using other web resources. It was useful to understand the code, and further resources can be found in the comments within the configure.ac and Makefile.am files, explaining each line.

## Credits
The cellID developers ([link to publication](http://dx.doi.org/10.1002/0471142727.mb1418s100))

The original source can be found at sourceforge ([link](https://sourceforge.net/projects/cell-id/))

## Dependencies
Disclaimer: it is possible that not all of the installed packages are strictly required. I have not yet checked, but surely the "-dev" ones are essential.

I have made use of autotools, and have installed the following packages:
* automake
* autoconf
* libtool

The configure.ac and Makefile.am files have been set to require libtiff5-dev, libopenlibm-dev, and libglib2.0-dev.

You may install these in Ubuntu systems by running:

`sudo apt-get install libglib2.0 libglib2.0-dev libtiff5 libtiff5-dev libopenlibm-dev`
    
These can also be satisfied in Arch Linux.

### Notes

You may find details on how the automake/conf files are configured by opening them and reading the comments.

Surprisingly, although packages are installed using one name (such as libtiff5) the name that configure.ac can find may be another one (such as libtiff-4).

## Build and Install

To build and install, please cd into the directory where the files are and run:

    autoreconf -fvi
    ./configure
    make -j8
    sudo make install

Please refer to the cellID documentation to learn how to use this program.

Cheers!
