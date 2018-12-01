//
//  file_operations.hpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#ifndef file_operations_hpp
#define file_operations_hpp

#include <stdio.h>

#include <iostream>
#include <string>

class FileOperations {
public:
  bool ReadFile(const char *filename, char *data);
  bool WriteFile(const char *filename, std::string *data);
  int OpenFile(const char* filename, int mode);
  bool IsFileExists(const char* filename);
  
  long GetFileSize(const char* filename);
  char* LoadFile(const char* filename);
  
private:
  std::string data_;
};

#endif /* file_operations_hpp */
