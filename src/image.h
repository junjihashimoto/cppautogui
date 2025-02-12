#ifndef IMAGECPP_H
#define IMAGECPP_H
#include <cstdlib>
#include "data.h"

extern "C"{
struct RGB{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char alpha;
};

}

struct image{
  RGB* data;
  int len;
  int width;
  int height;
  inline image():data(NULL),len(0),width(0),height(0){};
  inline RGB* operator [] (int i) const {return data+i*width;}
  inline operator RGB* () const {return data;}
};


template<class Type>
class CastArray{
  Type pointer;
  int size;
public:
  template<class OriType>
  inline CastArray(OriType p,int s):pointer(static_cast<OriType>(p)),size(s){}
  inline Type operator [] (int i) const {return pointer+i*size;}
  inline operator Type () const {return pointer;}
};

//image.cpp
extern int image_free(image& i);
extern int image_init(image& i,int height,int width);
extern int image_init(const image& oi,image& i);
extern int image_copy(const image& oi,image& i);
extern unsigned char ruchar(double v);

//bmp.cpp
extern int bmp2image(const ::data& file,image& i);
extern int image2bmp(const image& i,::data& file,int bpp=24,char** bmp_into=NULL,char** pos_data=NULL);
extern int rawbmp2image(const ::data& file,int height,int width,image& i);

//jpeg.cpp
extern int jpeg2image(const ::data& file,image& i);
extern int image2jpeg(const image& i,::data& file);

//png.cpp
#define LIBPNG
#ifdef LIBPNG
extern int png2image(const ::data& file,image& i);
#endif

#endif
