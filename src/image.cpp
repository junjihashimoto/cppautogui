#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cmath>
int
image_free(image& i){
  free(i.data);
  i.len=0;
  i.width=0;
  i.height=0;
  i.data=NULL;
  return 0;
}
int
image_init(image& i,int height,int width){
  image_free(i);
  i.data=(RGB*)malloc(height*width*sizeof(RGB));
  assert(i.data!=NULL);
  i.height=height;
  i.width=width;
  i.len=i.height*i.width;
  return 0;
}
int
image_init(const image& oi,image& i){
  image_free(i);
  i.height=oi.height;
  i.width=oi.width;
  i.len=oi.len;
  i.data=(RGB*)malloc(i.height*i.width*sizeof(RGB));
  assert(i.data!=NULL);
  return 0;
}
int
image_copy(const image& oi,image& i){
  image_init(oi,i);
  memcpy(i.data,oi.data,i.height*i.width*sizeof(RGB));
  return 0;
}

unsigned char
ruchar(double dbuf){
  if(dbuf>255)
    dbuf=255;
  else if(dbuf<0)
    dbuf=0;
  else
    dbuf=rint(dbuf);
  return (unsigned char)dbuf;
}

