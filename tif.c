/*

Cell-ID is intended to identify cells in images and to calculate a
number of statistics, including statistics derived from corresponding
fluorescence images.
Copyright (C) 2005 Andrew Gordon

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Andrew Gordon can be contacted at
agordon@molsci.org
Molecular Sciences Institute
2168 Shattuck Ave, 2nd Floor
Berkeley, CA  94704

**********************************************************************
Start-copyright-notice-for-libtiff
Libtiff software is used by Cell-ID for some of the reading in of
TIF image file data and also for creating new TIF files. Libtiff is
available at http://www.remotesensing.org/libtiff/. The libtiff software
was written by Sam Leffler while working for Silicon Graphics and
contains the following copyright notice:

   "Copyright (c) 1988-1997 Sam Leffler
    Copyright (c) 1991-1997 Silicon Graphics, Inc.

    Permission to use, copy, modify, distribute, and sell this software and
    its documentation for any purpose is hereby granted without fee, provided
    that (i) the above copyright notices and this permission notice appear in
    all copies of the software and related documentation, and (ii) the names
    of Sam Leffler and Silicon Graphics may not be used in any advertising or
    publicity relating to the software without the specific, prior written
    permission of Sam Leffler and Silicon Graphics.

    THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
    EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
    WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

    IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
    ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
    OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
    WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
    LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
    OF THIS SOFTWARE."

End-copyright-notice-for-Libtiff
*********************************************




*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <time.h>
#include "tif_routines.h"


//We'll write the data into c[][] and fl[][].  These are declared
//extern in tif_routines.h.

#define  xmax8  255.0
#define xmax16 65535.0

//Whether to invert c[][] or not (sometimes boundary of cells has deeper dark
//spots than light) (the inversion is c-->(max-c)).  1=yes, 0=no
//dark==NULL means no correction, otherwise points to an image to
//subtract from the data.
float *get_data_from_tif_file(char *file,
            int invert,
            float *dark,
            int *xmax_address,
            int *ymax_address){

  TIFF *tif;

  tdata_t buf;
  uint32 image_width,image_length,row,rowsperstrip;
  uint16 planarconfig,bitspersample;
  uint16 photometric; //whether black or white is high

  int scanline_size;

  float array_max;

  unsigned short *data_16;
  unsigned short *tmp_p;

  unsigned char *data_8;
  unsigned char *tmp_p8;

  //float c_max; //For inversion
  float *image_data;

  //Make the datetime_array[] two long because the TIFFGetField
  //call below wants a pointer to an array, and I want to make sure
  //that it doesn't attempt to write to datatime_array[1] also.
  char *datetime_array[]={"abcdefghijklmnopqrstuvwxyz.",
        "abcdefghijklmnopqrstuvwxyz."};
  char *datetime;
  int i;
  int xmax,ymax;
  int xmax_ymax;

  if((tif=TIFFOpen(file,"r"))==NULL){
    printf("Can't open file %s.\n",file);
    return NULL;
  }

  //Get data from file (code lifted from
  //http://www.libtiff.org/libtiff.html#Version)
  if(tif){

    //Get some header fields in case we want to write a tiff file
    TIFFGetField(tif,TIFFTAG_IMAGEWIDTH, &image_width);
    TIFFGetField(tif,TIFFTAG_IMAGELENGTH, &image_length);
    TIFFGetField(tif,TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif,TIFFTAG_DATETIME,datetime_array);
    datetime=datetime_array[0];

    //    printf("Photometric is %i.\n",photometric);
    //printf("Image Width and length in pixels = %i, %i\n",
    //     image_width,image_length);

    xmax=image_width;
    ymax=image_length;
    *xmax_address=xmax;
    *ymax_address=ymax;

    xmax_ymax=xmax*ymax;
    //Will put data in *image_data
    image_data=(float *)malloc(xmax_ymax*sizeof(float));

    TIFFGetField(tif,TIFFTAG_PLANARCONFIG, &planarconfig);
    //Don't really know what this is, but need it for writing TIFF below
    //printf("Planar Configuration = %i\n",planarconfig);

    TIFFGetField(tif,TIFFTAG_BITSPERSAMPLE, &bitspersample);
    //printf("Bits per pixel = %i\n",bitspersample);
    if((bitspersample!=8)&&(bitspersample!=16)){
      printf("Unknown bits per pixel size: %i.\n",bitspersample);
      return NULL;
    }

    TIFFGetField(tif,TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    //printf("Rows per strip = %i\n",rowsperstrip);

    scanline_size=TIFFScanlineSize(tif);
    //printf("Scsanline size = %i.\n",scanline_size);
    buf = _TIFFmalloc(scanline_size);

    //Allocate space for reading 16-bit data or 8-bit data
    if(bitspersample==16){
      data_16=(unsigned short *)malloc(xmax_ymax*sizeof(unsigned short));
      tmp_p=data_16;

      for(row=0;row<image_length;row++){
  TIFFReadScanline(tif,buf,row,1);
  memcpy(tmp_p,buf,scanline_size);
  tmp_p+=xmax;
      }

    }else{
      data_8=(unsigned char *)malloc(xmax_ymax*sizeof(unsigned short));
      tmp_p8=data_8;

      for(row=0;row<image_length;row++){
  TIFFReadScanline(tif,buf,row,1);
  memcpy(tmp_p8,buf,scanline_size);
  tmp_p8+=xmax;
      }

    }

    _TIFFfree(buf);

    TIFFClose(tif);
  }

  if(bitspersample==16){

    for(i=0;i<xmax_ymax;i++){
      image_data[i]=(float)data_16[i];
    }
    free(data_16);

  }else{
    for(i=0;i<xmax_ymax;i++){
      image_data[i]=(float)data_8[i];
    }
    free(data_8);
  }

  //Subtract off dark image if it was requested
  if (dark!=NULL){
    for(i=0;i<xmax_ymax;i++){
      image_data[i]-=(dark[i]);
    }
  }

  //Check if want to use inverted image
  if(invert==1){
    array_max=-1.0;
    for(i=0;i<xmax_ymax;i++){
      if(image_data[i]>array_max)array_max=image_data[i];
    }
    for(i=0;i<xmax_ymax;i++){
      image_data[i]=array_max-image_data[i];
    }
  }

  return image_data;
}

/******************************************************/
int output_data_to_tif_file(char *file,
                            float *output_data,
                            int xmax_data,
                            int ymax_data,
                            int *labels,
                            int type,
                            int bit_size,
                            int invert,
                            int mask_output){


  //Output array output_data to a file.
  //The array labels tells where to add boundaries, etc. (NULL for none)
  //(xmax_data,ymax_data)=size of input array (ie, "output_data")

  //uint32 rowsperstrip=8;
  uint32 planarconfig=1;
  uint16 bitspersample=8;
  uint32 image_width=xmax_data;
  uint16 photometric=1;

  float tmp;
  float onetmp;

  float array_max,array_min;
  float scale;

  tdata_t data_buf;

  TIFF *tif;

  int i,j,k;

  unsigned char *p_char;
  unsigned short *p_short;

  //  char *datetime;
  char datetime[50];
  time_t rawtime;
  struct tm * timeinfo;

  int u;

  // mask_mod: unique cell boundaries value will surely need enough bits, 8 bits (for 255 CellIDs) may not be enough.
  if (mask_output==1 && type==0) {
    bit_size=16;
    array_max=65535.0;
    array_min=0.0;
    scale=array_max-array_min; // mask_mod, originally: scale=1.0/(array_max-array_min);
  }
  else {
    //Get max and min
    array_max=0.0;
    array_min=1.0e15;
    for(j=0;j<ymax_data;j++){
      for(i=0;i<xmax_data;i++){
        u=j*xmax_data+i;
        if(output_data[u]>array_max)array_max=output_data[u];
        if(output_data[u]<array_min)array_min=output_data[u];
      }
    }
    if (array_max>array_min){
      scale=1.0/(array_max-array_min);
    }else{
      scale=0.0;
    }
  }

  bitspersample=(uint16)bit_size;

  //Value of one degree of grayness:
  if (bitspersample==8){
    onetmp=1.0/(scale*xmax8);
  }
  else{
    onetmp=1.0;
  }

  tif=TIFFOpen(file,"w");

  //Set some headers that need to be set
  TIFFSetField(tif,TIFFTAG_IMAGEWIDTH, image_width);
  TIFFSetField(tif,TIFFTAG_PLANARCONFIG, planarconfig);
  TIFFSetField(tif,TIFFTAG_BITSPERSAMPLE, bitspersample);
  TIFFSetField(tif,TIFFTAG_PHOTOMETRIC, photometric);
  //  TIFFSetField(tif,TIFFTAG_ROWSPERSTRIP, rowsperstrip);


  //Write out current time into file
  time(&rawtime); //rawtime=current time since Jan 1, 1970 in seconds
  timeinfo=localtime(&rawtime);
  //datetime=asctime(timeinfo);
  strftime(datetime,sizeof(datetime),"%Y:%m:%d %H:%M:%S",timeinfo);
  TIFFSetField(tif,TIFFTAG_DATETIME, datetime);

  //Allocate space for data (either 16-bit or 8-bit)
  if(bitspersample==8){
    data_buf = (unsigned char *)malloc(xmax_data);
    p_char=data_buf;
  }else if(bitspersample==16){
    data_buf = (unsigned short *)malloc(xmax_data*2);
    p_short=data_buf;
  }else{
    printf("Bits per sample too high: %i.\n",bitspersample);
    TIFFClose(tif);
    return 0;
  }

  for(j=0;j<ymax_data;j++){
    for(i=0;i<xmax_data;i++){
      //in bounds
      u=j*xmax_data+i;
      //Convert data to 8 bit or 16 bit
      tmp=output_data[u];
      if(invert==1){ //Flip values back from array_max-c[][]
        if(tmp>0.0)tmp=array_max-tmp;
      }
      if (labels!=NULL){
        //type determines what set of labels to write out
        k=labels[u];                       // "labels" corresponds to the "d" array in "add_boundary_points_to_data" (segment.c)
        if (type==0){                      // The default value for BF type and flat_cors is 0.
          if(k>=20){                       // As modified in segment.c, values of "k=labels[u]" >= 20 should be cell boundaries (a different "int" per cell starting at 20).
            tmp=array_max-(1+k-20)*onetmp; // In "add_boundary_points_to_data" CellIDs are offset by 20, so subtract that here.
                                           // Also add "1" since CellIDs are zero-indexed, and we want to reserve max_intensity for cell numbers.
                                           // Note that since in segment.c "d[(b*xmax+a)]=i+20" starts at 20, then labels[u]==19 can mean something else.
          }else if(k==found_border){       // tif_routines.h says: #define found_border 5, the default for cell boundaries if present.
 				    tmp=array_max;                 // this should not actually trigger, but wouldnt hurt, since boundary pixels intensity goes down from array_max-1
 				  }else if(k==found_border_a){
 				    tmp=array_max-onetmp;
 				  }else if(k==found_border_b){
 				    tmp=array_max-(2.0*onetmp);
 				  }else if(k==found_border_c){
 				    tmp=array_max-(3.0*onetmp);
 				  }else if(k==found_border_d){
 				    tmp=array_max-(4.0*onetmp);
 				  }else if(k==found_border_e){
 				    tmp=array_max-(5.0*onetmp);
 				  }else if(k==found_border_f){
 				    tmp=array_max-(6.0*onetmp);
 				  }else if(k==found_border_g){
 				    tmp=array_max-(7.0*onetmp);
          }else if(k==cell_label){         // tif_routines.h says: #define cell_label 6, the default for cell number labels if present.
              tmp=array_max;               // tmp=array_max-(15.0*onetmp);
          }else if(k==delete_pixel){
				    tmp=array_min;
          }else {
            // mask_mod: what is this if clause for?
            if(mask_output==1){
              tmp=array_min;
            }else {
              tmp=tmp;
            }
          }
        }else if (type==1){                // The default value for FL type is 1
          if(labels[u]==found_border){
            tmp=array_max;
          } else if(k>=20){
            tmp=array_max-(k-20)*onetmp;
          }

        }else if (type==2){                // The default value for third_image type is 2
          if(labels[u]==found_border){
            tmp=array_max;
          }else if (labels[u]==cell_nucleus){
            tmp=array_max-5.0;
          }
        }
      }

      if(bitspersample==8){
        *(p_char+i)=(unsigned char) ((tmp-array_min)*scale*xmax8);
      }else{
        if (tmp<0.0) tmp=0.0;
        if(tmp>xmax16)tmp=xmax16;
        *(p_short+i)=(unsigned short) tmp;
        //Original is assumed to be 16-bit data, so just replace it here.
      }
    }
    TIFFWriteScanline(tif,data_buf,j,1);
  }

  TIFFClose(tif);
  free(data_buf);
  return 1;
}
