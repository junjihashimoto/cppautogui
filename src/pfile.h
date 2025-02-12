#ifndef FILECPP_H
#define FILECPP_H

#ifdef WIN32

extern "C"{
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>
}
#include <cstdio>
typedef int FD;

typedef LONG OFF_T;

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

inline int
mkdir(const char* dir,int mode){
	return mkdir(dir);
}

#else //WIN32

extern "C"{
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
}
typedef int FD;

#endif  //WIN32
#endif // FILECPP_H
