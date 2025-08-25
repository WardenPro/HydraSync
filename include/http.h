#ifndef HTTP_H
#define HTTP_H

#include <string.h>
#include <signal.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

int http_server_init_event(const char* addr, int port);

#endif // HTTP_H