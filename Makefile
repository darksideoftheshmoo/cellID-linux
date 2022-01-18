# Guia para incluir cosas:
## -I <include directory>
## -L <library directory>
## -l <library name>
## The include directory will be location of the header files. 
## The library directory will be the location of the library (.a or .so)
## and the library name will be the name of the library file, without the leading 'lib' prefix or its extension (i.e. -lsal rather than -l libsal.a ).

# -Wall for all compiler warnings messages
# CFLAGS = -Wall
# CLIBS = -I../../inst/tiff410/libtiff/ -L../../inst/tiff410/r_build/libtiff/ -ltiff -lm

# gcc -Wall enables all compiler's warning messages.
CFLAGS = -Wall

# Para incluir libtiff "estatico" (el libtiff.a en vez del libtiff.so) tengo que saber que otras flags "-l" necesito
# Eso se puede consultar con: $ pkg-config --static --libs libtiff-4
# Para dynamic linking se consulta con: $  pkg-config --libs libtiff-4
## Para linking estático entonces necesito usar: "-ltiff -lzstd -llzma -ljbig -ljpeg -lz"
## Y parece que también necesitaba: "-lwebp"
## Para cellid hace falta: "-lm"
## Para rcell2 usé: CLIBS = -I../../inst/tiff410/libtiff/ ../../inst/tiff410/r_build/libtiff/libtiff.a -lzstd -llzma -ljpeg -lz -lm -lwebp
CLIBS = -ltiff -lzstd -llzma -ljpeg -lz -lm -lwebp

# Una variable con los targets-requisitos de cell,
# que también se pasa a "gcc" al compilar ese target ("cell"),
# y aparece en el target "clean" para limpiar el output.
objects = cell.o segment.o tif.o nums.o date_and_time.o fit.o fft.o fft_stats.o split_and_overlap.o contiguous.o fl_dist.o align_image.o flatten.o

# tiflib = tiff # esto lo saqué porque no hace falta ya

# El orden de los argumentos en gcc es importante.
## Las libs deben ir despues de los objetos .c/.o/... (o se descartan).
## Leer:  https://stackoverflow.com/questions/2624238/c-undefined-references-with-static-library
cell: $(objects)
	gcc -o cell $(CFLAGS) $(objects) $(CLIBS)

# Target original
#cell : $(objects)
#	gcc -o cell $(CFLAGS) -lm -l$(tiflib) $(objects)

cell.o: segment.h tif_routines.h date_and_time.h nums.h point.h image_type.h align_image.h split_and_overlap.h parameters.h

tif.o: tif_routines.h

segment.o: segment.h nums.h fit.h tif_routines.h fft_stats.h point.h image_type.h split_and_overlap.h contiguous.h fl_dist.h parameters.h flatten.h

contiguous.o: contiguous.h

nums.o: nums.h tif_routines.h

date_and_time.o: date_and_time.h

fit.o: fit.h

fft.o: fft.h complex.h

fft_stats.o: segment.h fft.h fft_stats.h point.h complex.h

split_and_overlap.o: split_and_overlap.h contiguous.h

fl_dist.o: fl_dist.h contiguous.h

align_image.o: align_image.h

flatten.o: flatten.h

clean:
	rm cell $(objects)

