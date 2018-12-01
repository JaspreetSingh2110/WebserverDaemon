//
//  connection_handler.hpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#ifndef connection_handler_hpp
#define connection_handler_hpp

#include <stdio.h>
#include <iostream>
#include "protocol.h"
#include <microhttpd.h>

#include "file_operations.hpp"

/*static int HandleConnection(void *cls,
                     struct MHD_Connection *connection,
                     const char *url,
                     const char *method,
                     const char *version,
                     const char *upload_data, size_t *upload_data_size, void **ptr);*/

class ConnectionHandler {
public:
  explicit ConnectionHandler(int port, Protocol protocol);
  ~ConnectionHandler();
  ConnectionHandler() {}
  
  bool Start();
  bool Stop();
  bool InitializeTls();
  static int HandleConnection(void *cls,
                              struct MHD_Connection *connection,
                              const char *url,
                              const char *method,
                              const char *version,
                              const char *upload_data, size_t *upload_data_size, void **ptr);
  
  std::string cert_;
  std::string key_;
private:
  int port_;
  Protocol protocol_;
  //std::unique_ptr<struct MHD_Daemon> daemon_;
  struct MHD_Daemon *daemon_;
  std::unique_ptr<FileOperations> file_operations_;

  
};

#endif /* connection_handler_hpp */
