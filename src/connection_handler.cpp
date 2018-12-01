//
//  connection_handler.cpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#include "connection_handler.hpp"
#include <microhttpd.h>

#include <vector>
static int constexpr FILE_SIZE = 4096;
char constexpr SERVERKEYFILE[] = "/Users/jaspreetsingh/Desktop/workspace/Practice/WebserverApplication/GitRepo/WebserverDaemon/Key/server.key";
char constexpr SERVERCERTFILE[] = "/Users/jaspreetsingh/Desktop/workspace/Practice/WebserverApplication/GitRepo/WebserverDaemon/Cert/server.pem";

char constexpr PAGE[] = "<html><head><title>Webserver</title></head><body>libmicrohttpd demo<body></html>";

ConnectionHandler::ConnectionHandler(
    int port, Protocol protocol):port_(port), protocol_(protocol) {
  daemon_ = nullptr;
  file_operations_.reset(new FileOperations());
}

ConnectionHandler::~ConnectionHandler() {
  if (daemon_) {
    MHD_stop_daemon (daemon_);
    daemon_ = nullptr;
  }
}

bool ConnectionHandler::Stop() {
  std::cout << "Stopping handler started for " << protocol_ << " protocol and on port " << port_ << std::endl;
  if (daemon_) {
    MHD_stop_daemon (daemon_);
    daemon_ = nullptr;
  }
  return true;
}

bool ConnectionHandler::InitializeTls() {
  char buffer[FILE_SIZE];
  if (!file_operations_->ReadFile(SERVERKEYFILE, buffer)) {
    std::cout << "The key file could not be read.\n";
    return false;
  }
  key_ = std::string(buffer);
  //std::cout << "Key size: " << key_.size();
  
  if (!file_operations_->ReadFile(SERVERCERTFILE, buffer)) {
    std::cout << "The certificate file could not be read.\n";
    return false;
  }
  cert_ = std::string(buffer);
  //std::cout << "CERT size: " << cert_.size();
  return true;
}

bool ConnectionHandler::Start() {
  std::cout << "Starting handler started for " << protocol_ << " protocol and on port " << port_ << std::endl;
  int connection_flags = MHD_USE_AUTO | MHD_USE_SELECT_INTERNALLY | MHD_USE_ERROR_LOG;
  
  //MHD_USE_IPv6 MHD_USE_DUAL_STACK MHD_USE_TCP_FASTOPEN;

  std::vector<struct MHD_OptionItem> options = {
    { MHD_OPTION_CONNECTION_LIMIT, 100, NULL },
    { MHD_OPTION_CONNECTION_TIMEOUT, 10, NULL }
  };
  
  if (protocol_ == HTTPS) {
    if (!InitializeTls()) {
      std::cout << "Failed to initialise TLS connection\n";
      return false;
    }
    connection_flags |= MHD_USE_TLS;
    
    options.push_back(MHD_OptionItem{ MHD_OPTION_HTTPS_MEM_KEY, 0, (void *) key_.c_str()});
    options.push_back(MHD_OptionItem{ MHD_OPTION_HTTPS_MEM_CERT, 0, (void *) cert_.c_str()});
  }
  
  options.push_back({ MHD_OPTION_END, 0, NULL });
  daemon_ = MHD_start_daemon (connection_flags,
                             port_,
                             NULL, NULL,
                              &ConnectionHandler::HandleConnection, NULL,
                             MHD_OPTION_ARRAY, options.data(), MHD_OPTION_END);
  
  if (!daemon_) {
    std::cout<< "Failed to create Handler!!\n";
    return false;
  }
  std::cout << "Handler started for " << protocol_ << " protocol and on port " << port_ <<std::endl;
  return true;
}

int ConnectionHandler::HandleConnection (void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   const char *version,
                   const char *upload_data,
                   size_t *upload_data_size,
                   void **ptr)
{
  static int aptr;
  //const char *me = cls;
  struct MHD_Response *response;
  int ret;
  (void)upload_data;       /* Unused. Silent compiler warning. */
  (void)upload_data_size;  /* Unused. Silent compiler warning. */
  
  printf ("New %s request for %s using version %s\n", method, url, version);
  //MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);
  
  if (0 != strcmp (method, "GET")) {
    printf ("Unhandled request type: %s.\n", method);
    return MHD_NO;              /* unexpected method */
  }
  
  if (&aptr != *ptr)
  {
    /* do never respond on first call */
    *ptr = &aptr;
    return MHD_YES;
  }
  *ptr = NULL;                  /* reset when done */
  
  //response = MHD_create_response_from_fd_at_offset64 (sbuf.st_size, fd, 0);
  response = MHD_create_response_from_buffer(strlen(PAGE), (void *) PAGE , MHD_RESPMEM_PERSISTENT);
  //MHD_add_response_header (response, "Content-Type", MIMETYPE);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  
  // response = MHD_create_response_from_buffer (strlen (me), (void *) me, MHD_RESPMEM_PERSISTENT);
  return ret;
}
