# cellID-linux

This is meant to aid compilation of cellID v1.4 in GNU/linux systems.

There is also _VCellID_, a friendlier and more automated graphical user interface for Cell-ID.

I became tired of fighting with unincluded dependencies, and so tried autotools by following [this](https://robots.thoughtbot.com/the-magic-behind-configure-make-make-install) tutorial and using other web resources. It was useful to understand the code, and further resources can be found in the comments within the configure.ac and Makefile.am files, explaining each line.

**Branch note**: this branch has removed the glib dependency, **it has not been thoroughly tested**.

## Credits

The cellID developers ([1](https://www.nature.com/articles/nmeth1008))([2](http://dx.doi.org/10.1002/0471142727.mb1418s100)).

The original source can be found at sourceforge ([link](https://sourceforge.net/projects/cell-id/)) and in the original publication ([link](https://www.nature.com/articles/nmeth1008#supplementary-information)).

## Dependencies

### Ubuntu

Disclaimer: it is possible that not all of the installed packages are strictly required. I have not yet checked, but surely the "-dev" ones are essential in Ubuntu.

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

### Arch

These dependencies can also be satisfied in Arch Linux, sorry for not providing detailed instructions. I got openlibm from AUR, the rest are provided by standard repos.

`sudo pacman -S automake autoconf libtool glib2 libtiff`

`aurman -S openlibm` OR `yay -S openlibm`

### Notes

You may find details on how the automake/conf files are configured by opening them and reading the comments.

Surprisingly for me, packages may be installed using one name (such as libtiff5) but configure.ac can find them using another one (such as libtiff-4).

## Build and Install

To build and install, please cd into the directory where the files are and run:

    autoreconf -fvi
    ./configure
    make -j8
    sudo make install

Please refer to the cellID documentation to learn how to use this program.

Cheers!

### Troubleshooting

If you get a "multiple definitions" error, this occurs with newer gcc versions.

Checkout the other branch: `multiple_definitions_fix`

That one compiles fine on GCC `10.2.0`.

## Usage

### Parameters

`$ cell -p parameters.txt`

Example parameters are in the `parameters_example.txt` file, and a full description is available in `parameters_description.txt` (copied from Gordon et. al. 2007).

Run `cell --help-all` for commandline help.

## Output

For convenience, I have copied the `out_all` columns' description to `output_descriptions.csv`.
