#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "pfile.h"
#include "data.h"


#define GETSIZE BUFSIZ

int
data_init(data& file){
  assert(file.head==NULL);
  assert(file.len==0);
  assert(file.memlen==0);
  file.head=(char*)malloc(GETSIZE*2);
  file.len=0;
  file.memlen=GETSIZE*2;
  file.pos=0;
  assert(file.head!=NULL);
  return 0;
}

int
data_init(data& file,int len){
  data_init(file);
  if(len>0){
    file.head=(char*)realloc(file.head,len);
    file.len=len;
    file.memlen=len;
    file.pos=0;
    assert(file.head!=NULL);
  }
  return 0;
}

int
data_push(data& file,const char* buf,int size){
  int nextlen=file.len+size;
  assert(file.head!=NULL);
  assert(file.len>=0);
  assert(size>=0);
  while(nextlen>file.memlen){
    file.head=(char*)realloc(file.head,file.memlen*2);
    file.memlen*=2;
    assert(file.head!=NULL);
  }
  memcpy(file.head+file.len,buf,size);
  file.len+=size;
  return 0;
}

int
data_add(data& file,int size){
  int nextlen=file.len+size;
  assert(file.head!=NULL);
  assert(file.len>=0);
  assert(size>=0);
  while(nextlen>file.memlen){
    file.head=(char*)realloc(file.head,file.memlen*2);
    file.memlen*=2;
    assert(file.head!=NULL);
  }
  file.len+=size;
  return 0;
}

int
data_free(data& file){
  free(file.head);
  file.head=NULL;
  file.len=0;
  file.memlen=0;
  return 0;
}

int
set_data(FD fd,const data& file){
  if(write(fd,file.head,file.len)<0)
    return -1;
  return 0;
}

int
get_data(FD fd,data& file,int len){
  int file_size=0,get_num=0,mem_size=0;
  char* read_pos;
  char* region;
  if(len<0){
    data_free(file);
    mem_size=GETSIZE*2;
    region=read_pos=(char*)malloc(mem_size);
    while((get_num=read(fd,read_pos,GETSIZE))>0){
      file_size+=get_num;
      read_pos+=get_num;
      assert((mem_size-file_size)>=0);
      if((mem_size-file_size)<GETSIZE){
	mem_size*=2;
	region=(char*)realloc(region,mem_size);
	read_pos=region+file_size;
      }
    }
    file.head=region;
    file.len=file_size;
    file.memlen=mem_size;
  }else{
    if(len!=file.memlen){
      data_free(file);
      file.head=(char*)malloc(len);
      file.len=file.memlen=len;
    }
    read_pos=file.head;
    int remains=len;
    int rw;
    while(remains>0){
      rw=(remains>=GETSIZE?GETSIZE:remains);
      get_num=read(fd,read_pos,rw);
      if(get_num<=0)
	break;
      file_size+=get_num;
      remains-=get_num;
      read_pos+=get_num;
      
    }
  }
  return get_num;
}


//int
//exist_file(const char* filename){
//  FILE* file=NULL;
//  file=fopen(filename,"r");
//  if(file!=NULL)
//    fclose(file);
//  return (file!=NULL?1:0);
//}
int
exist_file(const char* filename){
  struct stat buf;
  return stat(filename,&buf);
}

int
change_directory(const char* filename){
  return chdir(filename);
}
int
make_directory(const char* dirname){
  if(exist_file(dirname)<0)
    mkdir(dirname,0777);
  return 0;
}


int
data_save(const data& file,const char* file_name,bool overwrite){
  FD fd;
  if(!overwrite&&exist_file(file_name)>=0)
    return -1;
  if(strcmp(file_name,"/dev/stdout")==0){
    fd=STDOUT_FILENO;
#ifdef WIN32    
    setmode(fd,O_BINARY );
#endif
  }
#ifdef WIN32
  else if((fd=open(file_name,O_CREAT|O_WRONLY|O_BINARY))<0)
#else
  else if((fd=open(file_name,O_CREAT|O_WRONLY,0666))<0)
#endif
    return -1;

  for(int i=0;i<file.len/BUFSIZ;i++){
//	  printf("%d\n",BUFSIZ);
	  if(::write(fd,file.head+i*BUFSIZ,BUFSIZ)<0){
      close(fd);
      return -1;
    }
  }
  if(::write(fd,file.head+(file.len/BUFSIZ)*BUFSIZ,file.len%BUFSIZ)<0){
    close(fd);
    return -1;
  }
//  printf("%d\n",file.len%BUFSIZ);

  close(fd);
  return 0;
}

int
data_load(data& file,const char* file_name,int len){
  FD fd;
  if(strcmp(file_name,"/dev/stdin")==0){
    fd=STDIN_FILENO;
#ifdef WIN32    
    setmode(fd,O_BINARY );
#endif
  }else{
    if(exist_file(file_name)<0)
      return -1;
#ifdef WIN32
    if((fd=open(file_name,O_RDONLY|O_BINARY))<0)
#else
    if((fd=open(file_name,O_RDONLY))<0)
#endif
      return -1;

  }
  if(get_data(fd,file,len)<0){
    close(fd);
    return -1;
  }
  close(fd);
  return 0;
}
