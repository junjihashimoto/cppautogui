#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
extern "C"{
#include <png.h>
}
using namespace std;

#define PNG_BYTES_TO_CHECK 4
struct data_buf{
  ::data* file;
  int pos;
};

static void
read_data(png_structp png_ptr, png_bytep p, png_uint_32 s){
  data_buf *st = (data_buf*)png_get_io_ptr(png_ptr);
  int ret;
  if((unsigned int)(st->file->len-st->pos)>s){
    ret=s;
    for(int i=0;i<ret;i++)
      p[i]=*(st->file->head+st->pos+i);
    st->pos+=s;
  }else{
    ret=(st->file->len-st->pos);
    for(int i=0;i<ret;i++)
      p[i]=*(st->file->head+st->pos+i);
    st->pos=st->file->len;
  }
}

/* error handler */

static void
error_handler(png_structp png_ptr, png_const_charp error_msg){
  int *try_when_error = (int *)png_get_error_ptr(png_ptr);
  if (!strcmp(error_msg, "incorrect data check"))
    *try_when_error = 1;
//    else
//      err_message("png loader: %s\n", error_msg);
  longjmp(png_jmpbuf(png_ptr), 1);
}

static void
warning_handler(png_structp png_ptr, png_const_charp warning_msg){
}

int
png2image(const ::data& file,image& im){
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;
  png_bytep *image_array;
  int bit_depth, color_type, interlace_type;
  int try_when_error;
  unsigned int i;
  data_buf st = {const_cast<::data*>(&file),0};

  try_when_error = 0;

  /* Create and initialize the png_struct */
  if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
					(void *)&try_when_error,
					error_handler,
					warning_handler)) == NULL)
    return -1;

  /* Allocate/initialize the memory for image information. */
  if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return -1;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return try_when_error ? 0 : -1;
  }

  png_set_read_fn(png_ptr, (void*)&st, (png_rw_ptr)read_data);
  //  png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);
  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	       &interlace_type, NULL, NULL);

  im.width = width;
  im.height = height;
  im.len=im.width*im.height;

  if (bit_depth == 16)
    png_set_strip_16(png_ptr);
  else if (bit_depth < 8)
    png_set_packing(png_ptr);

  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand(png_ptr);

  if (color_type & PNG_COLOR_MASK_ALPHA)
    png_set_strip_alpha(png_ptr);

  png_read_update_info(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  assert(PNG_COLOR_TYPE_RGB==color_type||
	 PNG_COLOR_TYPE_RGB_ALPHA==color_type||
	 PNG_COLOR_TYPE_PALETTE==color_type);

  /* prepare image data, store palette if exists */
  int bpl;
  switch (color_type) {
  case PNG_COLOR_TYPE_RGB:
    bpl= width * 3;
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    bpl= width * 4;
    break;
  case PNG_COLOR_TYPE_PALETTE:
    bpl= width;
    break;
  default:
    assert(false);
  }

  /* allocate memory for returned image */
  im.data=(RGB*)malloc(im.width*im.height*sizeof(RGB));
  assert(im.data!=NULL);

  png_byte buf[height*bpl];
  /* allocate memory for pointer array */
  if ((image_array = (png_byte**)calloc(height, sizeof(png_bytep))) == NULL) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return -1;
  }

  for (i = 0; i < height; i++)
    image_array[i] = buf + png_get_rowbytes(png_ptr, info_ptr) * i;

  /* read image */
  png_read_image(png_ptr, image_array);

  int pos;
  switch (color_type) {
  case PNG_COLOR_TYPE_RGB:
    pos=0;
    for(int i=0;i<im.len;i++,pos+=3){
      im.data[i].r=buf[pos];
      im.data[i].g=buf[pos+1];
      im.data[i].b=buf[pos+2];
    }
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    pos=0;
    for(int i=0;i<im.len;i++,pos+=4){
      im.data[i].r=buf[pos];
      im.data[i].g=buf[pos+1];
      im.data[i].b=buf[pos+2];
    }
    break;
  case PNG_COLOR_TYPE_PALETTE:
    pos=0;
    {
      png_colorp palette;
      int num;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num);
      for(int i=0;i<im.len;i++,pos++){
	im.data[i].r=palette[buf[pos]].red;
	im.data[i].g=palette[buf[pos]].green;
	im.data[i].b=palette[buf[pos]].blue;
      }
    }
    break;
  default:
    assert(false);
  }

  /* read rest of file, and get additional chunks in info_ptr */
  png_read_end(png_ptr, info_ptr);

  /* clean up after the read, and free any memory allocated */
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  /* free temporary memory */
  free(image_array);
  return 0;
}

#ifdef PNGCPP_MAIN
int
main(){
  data file;
  image im;
  data_load(file,"/dev/stdin");
  png2image(file,im);
  data_free(file);
  RGB (*dbuf)[im.width];
  dbuf=(RGB (*)[im.width])(im.data);
  for(int i=0;i<im.height;i++){
    for(int j=0;j<im.width;j++){
      dbuf[i][j].r=255-dbuf[i][j].r;
      dbuf[i][j].g=255-dbuf[i][j].g;
      dbuf[i][j].b=255-dbuf[i][j].b;
    }
  }
  //  std::reverse(im.data.begin(),im.data.end());
  image2ppm(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}
#endif
