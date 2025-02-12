#include "image.h"
#include <cassert>
#include <cstdlib>


int
rawbmp2image(const data& file,int height,int width,image& i){
  //  if(file.len!=3*height*width)
  //    return -1;
  if(i.len!=height*width)
    image_init(i,height,width);

  CastArray<RGB*> d(i.data,i.width);
  CastArray<unsigned char (*)[3]> d2((unsigned char(*)[3])file.head,width);
  
  for(int h=0;h<height;h++)
    for(int w=0;w<width;w++){
      if(3*(w+h*width)<file.len){
	d[h][w].r=d2[h][w][0];
	d[h][w].g=d2[h][w][1];
	d[h][w].b=d2[h][w][2];
      }else{
	d[h][w].r=0;
	d[h][w].g=0;
	d[h][w].b=0;
      }
    }
  return 0;
}

int
image2rawbmp(const image& i,data& file){
  /*
  typedef unsigned int uint;
  //  assert(false);
  data_init(file,i.len*3/2);
  CastArray<RGB*> d(i.data,i.width);
  CastArray<unsigned char *> y((unsigned char*)file.head,i.width);
  */
  return 0;
}

int
bmp2image(const data& file,image& i){
  int bpp;
  unsigned char *begin,*pos;
  unsigned char *parret;
  int offset;
  assert(file.len>=54);
  assert(file.head[0]=='B');
  assert(file.head[1]=='M');
  assert(*(long int*)(file.head+2)==file.len);
  offset=*(long int*)(file.head+10);
  assert(*(long int*)(file.head+14)==40);
  i.width=*(long int*)(file.head+18);
  i.height=*(long int*)(file.head+22);
  assert(*(short int*)(file.head+26)==1);
  bpp=*(short int*)(file.head+28);
  assert(*(long int*)(file.head+30)==0);
  //  assert(*(long int*)(file.head+34)==file.len-offset);
  assert(file.len-offset==(i.width *bpp/8+(4-(i.width * bpp/8)%4)%4)*i.height);
  begin=(unsigned char*)file.head+offset;
  parret=(unsigned char*)file.head+54;
  i.len=i.width*i.height;
  i.data=(RGB*)malloc(i.width*i.height*sizeof(RGB));
  assert(i.data!=NULL);
  
//    RGB (*dbuf)[i.width];
//    dbuf=(RGB (*)[i.width])(i.data);
//    for(int h=i.height-1;h>=0;h--){
  unsigned char ppos;
  for(int h=0;h<i.height;h++){
    pos=begin+(i.width * bpp/8+(4-(i.width * bpp/8)%4)%4)*(i.height-h-1);
    for(int w=0;w<i.width;w++){
      RGB rgb;
      switch(bpp){
      case 8:
	ppos=*pos;
	rgb.b=*(parret+(ppos*4));
	rgb.g=*(parret+(ppos*4)+1);
	rgb.r=*(parret+(ppos*4)+2);
	rgb.alpha=*(parret+(ppos*4)+3);
	pos++;
	break;
      case 16:
	unsigned short int p;
	p=*(unsigned short int*)pos;
	rgb.b=0x1f&p;
	rgb.g=(0x3e0&p)>>5;
	rgb.r=(0x7c00&p)>>10;
	*pos+=2;
	break;
      case 24:
	rgb.b=*pos++;
	rgb.g=*pos++;
	rgb.r=*pos++;
	break;
      case 32:
	rgb.b=*pos++;
	rgb.g=*pos++;
	rgb.r=*pos++;
	rgb.alpha=*pos++;
	break;
      default:
	return -1;
      }
      i.data[i.width*h+w]=rgb;
//        dbuf[h][w]=rgb;
    }
  }
  return 0;
}
int
image2bmp(const image& im,data& file,int bpp,char** bmp_info,char** pos_data){
  long int li=0;
  short int si=0;
  int pinfo,pdata;
  
  data_init(file);
  data_push(file,"BM",2);
  li=54+(im.width *bpp/8+(4-(im.width * bpp/8)%4)%4)*im.height;
  data_push(file,(char*)&li,4);    
  data_push(file,(char*)&si,2);
  data_push(file,(char*)&si,2);
  li=54;
  data_push(file,(char*)&li,4);

  pinfo=file.len;
  
  li=40;
  data_push(file,(char*)&li,4);
  li=im.width;
  data_push(file,(char*)&li,4);
  li=im.height;
  data_push(file,(char*)&li,4);
  si=1;
  data_push(file,(char*)&si,2);
  si=bpp;
  data_push(file,(char*)&si,2);
  li=0;
  data_push(file,(char*)&li,4);
  li=(im.width *bpp/8+(4-(im.width * bpp/8)%4)%4)*im.height;
  data_push(file,(char*)&li,4);
  
  li=0;
  data_push(file,(char*)&li,4);
  data_push(file,(char*)&li,4);
  data_push(file,(char*)&li,4);
  data_push(file,(char*)&li,4);

  pdata=file.len;

  RGB* pos;
  int diff=(4-(im.width*(bpp/8))%4)%4;
  for(int h=im.height-1;h>=0;h--){
    pos=im.data+im.width*h;
    for(int w=0;w<im.width;w++){
      RGB rgb=pos[w];
      switch(bpp){
      case 24:
	data_push(file,(char*)&(rgb.b),1);
	data_push(file,(char*)&(rgb.g),1);
	data_push(file,(char*)&(rgb.r),1);
	break;
      case 32:
	data_push(file,(char*)&(rgb.b),1);
	data_push(file,(char*)&(rgb.g),1);
	data_push(file,(char*)&(rgb.r),1);
	data_push(file,(char*)&(rgb.alpha),1);
	break;
      default:
	return -1;
      }
    }
    for(int j=0;j<diff;j++){
      char c=0;
      data_push(file,&c,1);
    }
  }

  if(bmp_info!=NULL)
    *bmp_info=file.head+pinfo;
  if(pos_data!=NULL)
    *pos_data=file.head+pdata;

  return 0;
}
#ifdef BMPCPP_MAIN
int
main(){
  data file;
  image im;
  data_load(file,"/dev/stdin");
  bmp2image(file,im);
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
  //  std::reverse(i.data.begin(),i.data.end());
  image2bmp(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}

#endif
#ifdef BMPCPP_MAIN2
int
main(){
  data file;
  image_array im;
  data_load(file,"/dev/stdin");
  bmp2image(file,im);
  data_free(file);
  for(int i=0;i<im.height;i++){
    for(int j=0;j<im.width;j++){
      im[i][j].r=255-im[i][j].r;
      im[i][j].g=255-im[i][j].g;
      im[i][j].b=255-im[i][j].b;
    }
  }
  //  std::reverse(i.data.begin(),i.data.end());
  image2bmp(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}

#endif
