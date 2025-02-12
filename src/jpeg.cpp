#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

extern "C"{
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>

typedef struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
} *my_error_ptr;

typedef struct {
  struct jpeg_source_mgr pub;	/* public fields */
  data*  file;		/* source stream */
  JOCTET * buffer;		/* start of buffer */
  boolean start_of_file;	/* have we gotten any data yet? */
  int pos;
  JOCTET  end[2];
} my_source_mgr;
typedef my_source_mgr * my_src_ptr;

typedef struct {
  struct jpeg_destination_mgr pub; /* public fields */
  data * file;		/* target stream */
  JOCTET * buffer;		/* start of buffer */
  int pos;
  JOCTET  end[2];
} my_destination_mgr;
typedef my_destination_mgr * my_dest_ptr;
}
#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */

void
my_error_exit(j_common_ptr cinfo){
  my_error_ptr myerr;
  myerr = (my_error_ptr)cinfo->err;
  (*cinfo->err->output_message)(cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

void
my_error_output(j_common_ptr cinfo){
  my_error_ptr myerr;
  char buf[JMSG_LENGTH_MAX];
  myerr = (my_error_ptr)cinfo->err;
  (*cinfo->err->format_message)(cinfo, buf);
  fprintf(stderr, "ERR: %s\n", buf);
}


METHODDEF(void)
term_source (j_decompress_ptr cinfo){}
METHODDEF(void)
init_source (j_decompress_ptr cinfo){
  my_src_ptr src = (my_src_ptr) cinfo->src;
  src->start_of_file = TRUE;
  src->end[0] = (JOCTET) 0xFF;
  src->end[1] = (JOCTET) JPEG_EOI;
  src->pos=0;
}
METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  size_t nbytes;
  JOCTET* buf;
  if(src->pos==src->file->len)
    nbytes=0;
  else{
    buf=reinterpret_cast<JOCTET*>(src->file->head);
    src->pos=src->file->len;
    nbytes=src->file->len;
  }
  if (nbytes <= 0) {
    if (src->start_of_file)	/* Treat empty input file as fatal error */
      ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->pub.next_input_byte = src->end;
    nbytes = 2;
  }else
    src->pub.next_input_byte = buf;
  src->pub.bytes_in_buffer=nbytes;
  src->start_of_file = FALSE;
  return TRUE;
}

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


void
jpeg_data_src (j_decompress_ptr cinfo,const data& file){
  my_src_ptr src;
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));
    src = (my_src_ptr) cinfo->src;
  }
//    src->buffer = (JOCTET *)
//        (*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
//                                   8196 * sizeof(JOCTET));
   src->buffer = (JOCTET *)NULL;
  src = (my_src_ptr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->file = const_cast<data*>(&file);
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}
template<class T>
class auto_free{
public:
  T* ptr;
  auto_free(T* p):ptr(p){};
  auto_free(void* p):ptr(static_cast<T*>(p)){};
  ~auto_free(){free(ptr);}
};

int
jpeg2image(const data& file,image& i){
  JSAMPROW p[1];
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  jerr.pub.output_message = my_error_output;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    //assert(false);
    return -1;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_data_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);
  
  jpeg_calc_output_dimensions(&cinfo);
  i.width = cinfo.output_width;
  i.height = cinfo.output_height;
  /*
  if (cinfo.output_components != 3) {
    jpeg_destroy_decompress(&cinfo);
    //assert(false);
    return -1;
  }
  */
  i.len=i.width*i.height;
  i.data=(RGB*)malloc(i.width*i.height*sizeof(RGB));
  assert(i.data!=NULL);
  
  unsigned char *buf=(unsigned char*)malloc(sizeof(unsigned char)*i.width*i.height*cinfo.output_components);
  auto_free<unsigned char> af(buf);
  assert(buf!=NULL);
  
  jpeg_start_decompress(&cinfo);
  while (cinfo.output_scanline < cinfo.output_height) {
    p[0] = (JSAMPROW)&buf[cinfo.output_scanline * i.width * cinfo.output_components];
    jpeg_read_scanlines(&cinfo, p, 1);
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  unsigned char *pos=buf;
  switch(cinfo.output_components){
  case 1:
    if (cinfo.out_color_space == JCS_RGB){
      for(int j=0;j<i.len;j++,pos++){
	i.data[j].r=GETJSAMPLE(cinfo.colormap[0][pos[0]]);
	i.data[j].g=GETJSAMPLE(cinfo.colormap[1][pos[0]]);
	i.data[j].b=GETJSAMPLE(cinfo.colormap[2][pos[0]]);
	i.data[j].alpha=0;
      }
    }else if(cinfo.out_color_space == JCS_GRAYSCALE){
      for(int j=0;j<i.len;j++,pos++){
	i.data[j].r=i.data[j].g=i.data[j].b=pos[0];
	i.data[j].alpha=0;
      }
    }else
      assert(false);
    break;
  case 3:
    for(int j=0;j<i.len;j++,pos+=3){
      i.data[j].r=pos[0];
      i.data[j].g=pos[1];
      i.data[j].b=pos[2];
      i.data[j].alpha=0;
    }
    break;
  default:
    assert(false);
  }
  return 0;
}

METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  dest->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  OUTPUT_BUF_SIZE * sizeof(JOCTET));
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}
METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  data_push(*(dest->file),reinterpret_cast<char*>(dest->buffer),OUTPUT_BUF_SIZE);
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
  return TRUE;
}
METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
  if (datacount > 0)
    data_push(*(dest->file),reinterpret_cast<char*>(dest->buffer),datacount);
}
void
jpeg_data_dest (j_compress_ptr cinfo,const data& file){
  my_dest_ptr dest;
  if (cinfo->dest == NULL) {	/* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_destination_mgr));
  }
  dest = (my_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->file = const_cast<data*>(&file);
}
  

int
image2jpeg(const image& i,data& file){
  data_init(file);
  JSAMPROW p[1];
  struct jpeg_compress_struct cinfo;
  struct my_error_mgr jerr;
  char *buf=(char*)malloc(i.width*i.height*3);
  char* pos=buf;
  assert(buf!=NULL);
  for(int j=0;j<i.len;j++,pos+=3){
    pos[0]=i.data[j].r;
    pos[1]=i.data[j].g;
    pos[2]=i.data[j].b;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  jerr.pub.output_message = my_error_output;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_compress(&cinfo);
    //assert(false);
    free(buf);
    return -1;
  }

  jpeg_create_compress(&cinfo);
  jpeg_data_dest(&cinfo, file);
  cinfo.image_width = i.width;
  cinfo.image_height = i.height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);

  jpeg_start_compress(&cinfo, TRUE);

  while (cinfo.next_scanline < cinfo.image_height) {
    p[0] = (JSAMPROW)&buf[cinfo.next_scanline * i.width * 3];
    jpeg_write_scanlines(&cinfo, p, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  free(buf);
  return 0;
}

#ifdef JPEGCPP_MAIN
int
main(){
  data file;
  image im;
  data_load(file,"/dev/stdin");
  jpeg2image(file,im);
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
  image2jpeg(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}

#endif

