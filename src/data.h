#ifndef DATACPP_H
#define DATACPP_H

#if defined __CYGWIN32__ && !defined __CYGWIN__
#define __CYGWIN__ __CYGWIN32__ 
#endif

#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <cstdlib>
#include <string>

#include "pfile.h"

struct data{
  char* head;
  int len;
  int memlen;
  int pos;
  std::string misc;
  inline data():head(NULL),len(0),memlen(0),pos(0){};
};

//data.cpp
extern int data_init(::data& file);
extern int data_init(::data& file,int len);
extern int data_push(::data& file,const char* buf,int size);
extern int data_add(::data& file,int size);
extern int data_free(::data& file);
extern int set_data(FD fd,const ::data& file);
extern int get_data(FD fd,::data& file,int len=-1);
extern int exist_file(const char* filename);//if file does not exist,return -1.
extern int change_directory(const char* filename);
extern int make_directory(const char* dirname);
extern int data_save(const ::data& file,const char* file_name,bool overwrite=true);
extern int data_load(::data& file,const char* file_name,int len=-1);
template<class Type>
int
dget(::data& file,Type& c){
  if(!(file.pos>=0&&file.pos+(int)sizeof(Type)<=file.len))
    return -1;
  c=*reinterpret_cast<Type*>(file.head+file.pos);
  file.pos+=sizeof(Type);
  return 0;
}
inline int
dgetstring(::data& file,std::string& c,int len){
  if(!(file.pos>=0&&file.pos+len<=file.len))
    return -1;
  c.assign(file.head+file.pos,len);
  file.pos+=len;
  return 0;
}
template<class Type>
int
dset(::data& file,const Type& c){
  if(file.pos+(int)sizeof(c)>file.len&&data_add(file,(int)sizeof(c))<0)
    return -1;
  *reinterpret_cast<Type*>(file.head+file.pos)=c;
  file.pos+=sizeof(Type);
  return 0;
}
inline int
dsetstring(::data& file,const std::string& c,int len){
  if(file.pos+len>file.len&&data_add(file,len)<0)
    return -1;
  //  if(data_add(file,len)<0)
  //    return -1;
  c.copy(file.head+file.pos,len);
  file.pos+=len;
  return 0;
}

#endif //DATACPP_H
