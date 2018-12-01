//
//  webserver_daemon.h
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#ifndef webserver_daemon_h
#define webserver_daemon_h

#include <stdio.h>
#include <iostream>

#include <microhttpd.h>

#include "connection_handler.hpp"

class WebserverDaemon {
 public:
  int OnInit();
private:
  bool daemon_started_;
  std::string service_name_;
  ConnectionHandler http_connection_handler_;
  ConnectionHandler https_connection_handler_;
};

#endif /* webserver_daemon_h */
