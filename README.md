# cellID-linux

This is meant to aid compilation of CellID v1.4 in GNU/linux systems.

Please read the `README.txt` file, which contains key information on:

1. Compiling Cell-ID
2. Running Cell-ID
3. Outputs from Cell-ID
4. Brief description of code (though it is indeed brief, the outline is very useful for modifications)
5. Data analysis package PAW

There is also _VCellID_, a friendlier and more automated graphical user interface for Cell-ID.

Also under development are the functions in our `rcell2` R package, which streamline the single-cell segmentation and analysis workflow, entirely within R.

I became tired of fighting with unincluded dependencies, and so tried autotools by following [this](https://robots.thoughtbot.com/the-magic-behind-configure-make-make-install) tutorial and using other web resources. It was useful to understand the code, and further resources can be found in the comments within the configure.ac and Makefile.am files, explaining each line.

# Branch notes

This branch has removed the glib dependency, **it has not been thoroughly tested**.

This branch outputs BF tiff files with additional mask functionality for custom mask outputs. **it has not been thoroughly tested**. These options are disabled by default, but can be enabled by including the following parameters when calling `cell` from the command line:

* `-l`: sets mask boundary pixel intensities proportional to each cellID, following the relationship `cellID = 65535 - boundary_intensity - 1` (see image 1 at the end of the readme), and also adds cellID numbers to the cells, with maximum pixel intensity (`65535`). 

* `-i`: sets mask boundary **and** interior pixel intensities proportional to each cellID. This overrides cell labeling.

* `-w`: offsets boundary and interior pixel intensities by `10000`, so that boundary pixels follow the standard cellID relationship, and interior pixels the relationship `cellID = 65535 - boundary_intensity - 10000 - 1`.

* `-m`: sets blank background. Default output is cell boundaries only, but can be modified with `-l` (labels), `-i` (interior), and `-w` (offset).

Another way of getting the boundary and interior points is with the `-t` option. This makes `cell` create a new output file with a table of x/y coordinates for all cell boundary and interior pixels (each identified with `cellID`, `t.frame`, `flag`, and `pixtype`).

## Credits

The cellID developers ([1](https://www.nature.com/articles/nmeth1008))([2](http://dx.doi.org/10.1002/0471142727.mb1418s100)).

The original source can be found at sourceforge ([link](https://sourceforge.net/projects/cell-id/)) and in the original publication ([link](https://www.nature.com/articles/nmeth1008#supplementary-information)).

## Dependencies

### Ubuntu Linux

Disclaimer: it is possible that not all of the installed packages are strictly required. I have not yet checked, but surely the "-dev" ones are essential in Ubuntu.

I have made use of autotools, and have installed the following packages:

* automake: 1:1.15-4ubuntu1
* autoconf: 2.69-9
* libtool: 2.4.6-0.1

You may install these in Ubuntu systems by running:

`sudo apt-get install automake autoconf libtool`

The `configure.ac` and `Makefile.am` files have been set to require `libtiff5-dev`, `libopenlibm-dev`, and `libglib2.0-dev` (this last dependency was removed from this branch).

* libglib2.0-dev: 2.48.2-0ubuntu1
* libtiff5-dev: 4.0.6-1ubuntu0.4
* libopenlibm-dev: 0.5.0+dfsg-2

You may install these in Ubuntu systems by running:

`sudo apt-get install libglib2.0 libglib2.0-dev libtiff5 libtiff5-dev libopenlibm-dev`

### Arch Linux

These dependencies can also be satisfied in Arch Linux, sorry for not providing detailed instructions. I got openlibm from AUR, the rest are provided by standard repos.

`sudo pacman -S automake autoconf libtool glib2 libtiff`

`aurman -S openlibm` OR `yay -S openlibm`

### macOS
It may be necessary to install Apple's Command Line Tools for Cell-ID to compile successfully. Try the instructions below, and if you are unable to compile, install Command Line Tools and try again.

Use `brew` (Homebrew) to install the following:

* `autoconf`, `automake`, and `libtool` are the compilation tools:
    `brew install autoconf automake libtool`

* `libtiff` and `openlibm` are CellID dependencies:
    `brew install libtiff openlibm`

Note: `openlibm` is apparently bundled with the OS, but I was unable to make to compiler find this installation. 

### Notes

You may find details on how the automake/conf files are configured by opening them and reading the comments.

Surprisingly for me, packages may be installed using one name (such as libtiff5) but configure.ac can find them using another one (such as libtiff-4).

## Build and Install

### Linux

To build and install, please cd into the directory with the CellID files and run:

    autoreconf -fvi
    ./configure
    make -j8
    sudo make install

### macOS

`openlibm` is not found by `pkg-config` automatically, and needs to be manually configured. Run this first:

    export PKG_CONFIG_PATH="/usr/local/opt/openlibm/lib/pkgconfig"

Then, to build and install, please `cd` into the directory with the CellID files and run:

    autoreconf -fvi
    ./configure
    make -j4
    sudo make install

### Troubleshooting compilation

If you get a "multiple definitions" error, this occurs with newer gcc versions.

Checkout the other branch: `multiple_definitions_fix`

That one and its derivatives compile fine on GCC `10.2.0`.

## Usage

`$ cell -p parameters.txt -o /path/to/output/dir -b bf_images_list.txt -f fl_images_list.txt -l`

Each `cell` execution will deal with one "position" at a time.

Please refer to the cellID documentation to learn how to use this program.

Cheers!

### Command-line arguments

Example parameters are in the `parameters_example.txt` file, and a full description is available in `parameters_description.txt` (copied from Gordon et al 2007) and in the `README.txt` file.

In short:

The `-o` option refers to the output directory.

The `-b` and `-f` receive paths to text files containing BF and FL file paths (one per line, ordered by t.frame first and channel second).

The `-p` option specifies the path to a "parameters" file, that is used to set CellID segmentation options.

The `-h` flag tells the user to read this file, and the `README.txt` file.

### CellID parameters

The `parameters_description.txt` file contains valid parameters names and values, and description for each one.

The `parameters_example.txt` contains a brief example of typical settings.

The `README.txt` can also be helpful.

For more extensive details, you may want to visit the original publications:

* CellID: 10.1038/nmeth1008
* V-CellID / Rcell: 10.1002/0471142727.mb1418s84 and 10.1002/0471142727.mb1418s100

The ultimate source of parameters usage, is the C source code of the program; specifically the `cell.c` source file.

## Output description

The main CellID output is typically an "out_all" TSV file for each microscope position. A description for each column is available, please read:

* `output_descriptions2.txt`
* `output_descriptions.csv`
* `README.txt`


# Appendix

### Image 1

Note that the value for the boundary of this cell with `id = 0` is `65534`, which is consistent: `boundary_intensity = 65535 - cellID - 1`.

![cellid_intensity_relationship](doc/cellid_intensity_relationship.png)
