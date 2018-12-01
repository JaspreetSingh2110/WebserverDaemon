//
//  file_operations.cpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#include "file_operations.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

static int constexpr FILESIZE = 4096;

bool FileOperations::IsFileExists(const char *filename) {
  struct stat stat_buff;
  if (stat(filename, &stat_buff) == 0) {
    return true;
  }
  return false;
}

int FileOperations::OpenFile(const char *filename, int mode) {
  int fd = open(filename, mode);
  return fd;
}

bool FileOperations::ReadFile(const char *filename, char *data) {
  if (!IsFileExists(filename)) {
    std::cout << "File: " << filename << " does not exist.\n";
    return false;
  }
  
  int fd;
  if ((fd = OpenFile(filename, O_RDONLY)) == -1) {
    std::cout << "Failed to open file: " << filename << std::endl;
    return false;
  }
  
  if (read(fd, data, FILESIZE) <= 0) {
    std::cout << "Failed to read file data.\n";
    close(fd);
    return false;
  }
  close(fd);
  return true;
}

bool FileOperations::WriteFile(const char *filename, std::string *data) {
  // TODO: Add File Write functionality
  return true;
}

long FileOperations::GetFileSize(const char *filename)
{
  FILE *fp;
  fp = fopen(filename, "rb");
  if (fp) {
    long size;
    if ((0 != fseek (fp, 0, SEEK_END)) || (-1 == (size = ftell (fp)))) {
      size = 0;
    }
    fclose (fp);
    return size;
  }
  
  return 0;
}

char* FileOperations::LoadFile(const char *filename)
{
  FILE *fp;
  char *buffer;
  long size;
  
  if ((size = GetFileSize(filename)) == 0) {
    printf ("File: %s is empty\n", filename);
    return NULL;
  }
  
  fp = fopen (filename, "rb");
  if (!fp) {
    return NULL;
  }
  
  buffer = (char *) malloc(size + 1);
  if (!buffer) {
    fclose(fp);
    return NULL;
  }
  
  buffer[size] = '\0';
  if (size != (long) fread(buffer, 1, size, fp)) {
    free (buffer);
    buffer = NULL;
  }
  
  fclose (fp);
  return buffer;
}
