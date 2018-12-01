//
//  webserver_daemon.cpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#include "webserver_daemon.h"

int WebserverDaemon::OnInit() {
  std::cout << "Webserver Daemon Initializing\n";
  ConnectionHandler http_connection_handler_(8080, Protocol::HTTP);
  http_connection_handler_.Start();
  ConnectionHandler https_connection_handler_(8081, Protocol::HTTPS);
  https_connection_handler_.Start();
  sleep(10);
  http_connection_handler_.Stop();
  https_connection_handler_.Stop();
  return 1;
}
