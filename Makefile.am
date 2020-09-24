bin_PROGRAMS = cell
cell_SOURCES = cell.c align_image.c contiguous.c date_and_time.c fft.c fft_stats.c fit.c flatten.c fl_dist.c nums.c oif.c segment.c split_and_overlap.c tif.c

# http://smalltalk.gnu.org/blog/bonzinip/all-you-should-really-know-about-autoconf-and-automake
# On top of this, for each package you need, you add:

AM_CFLAGS = $(libtiff_CFLAGS) $(libm_CFLAGS)
LIBS += $(libtiff_LIBS) $(libm_LIBS)
