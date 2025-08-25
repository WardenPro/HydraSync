#ifndef HTTP_H
#define HTTP_H
#define UNUSED(x) (void)(x)
#include <string.h>
#include <signal.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <uuid/uuid.h>

struct session_store {
    int session_token;
};

struct route_entry {
    const char *path;
    void (*handler)(struct evhttp_request *, void *arg);
    ev_uint32_t methods;
};

int http_server_init_event(const char* addr, int port);
void health_handler(struct evhttp_request *req, void *ctx);
void sessions_handler(struct evhttp_request *req, void *ctx);

#endif // HTTP_H