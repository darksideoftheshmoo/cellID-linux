# cellID-linux
This is meant to aid compilation of cellID v1.4 in GNU/linux systems

I became tired of fighting with unincluded dependencies, and so tried autotools by following [this](https://robots.thoughtbot.com/the-magic-behind-configure-make-make-install) tutorial and using other web resources. It was useful to understand the code, and further resources can be found in the comments within the configure.ac and Makefile.am files, explaining each line.

## Credits
The cellID developers ([link to publication](http://dx.doi.org/10.1002/0471142727.mb1418s100))

The original source can be found at sourceforge ([link](https://sourceforge.net/projects/cell-id/))

## Dependencies
Disclaimer: it is possible that not all of the installed packages are strictly required. I have not yet checked, but surely the "-dev" ones are essential.

I have made use of autotools, and have installed the following packages:
* automake: 1:1.15-4ubuntu1
* autoconf: 2.69-9
* libtool: 2.4.6-0.1

You may install these in Ubuntu systems by running:

`sudo apt-get install automake autoconf libtool`

The configure.ac and Makefile.am files have been set to require libtiff5-dev, libopenlibm-dev, and libglib2.0-dev.
* libglib2.0-dev: 2.48.2-0ubuntu1
* libtiff5-dev: 4.0.6-1ubuntu0.4
* libopenlibm-dev: 0.5.0+dfsg-2

You may install these in Ubuntu systems by running:

`sudo apt-get install libglib2.0 libglib2.0-dev libtiff5 libtiff5-dev libopenlibm-dev`

These dependencies can also be satisfied in Arch Linux, sorry for not providing detailed instructions.
`sudo pacman -S automake autoconf libtool glib2 libtiff`
`aurman -S openlibm`
I got openlibm from AUR, the rest are provided by standard repos.

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
