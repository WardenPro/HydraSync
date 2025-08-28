#ifndef HTTP_H
#define HTTP_H
#define UNUSED(x) (void)(x)
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <uuid/uuid.h>

typedef struct session {
    char session_token[37]; // UUID string format
}session;

typedef struct session_store {
    session *sessions;
    size_t count;
    size_t capacity;
}session_store;

typedef struct route_entry {
    const char* path;
    void (*handler)(struct evhttp_request *, void *arg);
    ev_uint32_t methods;
}route_entry;

int http_server_init_event(const char* addr, int port);
void health_handler(struct evhttp_request *req, void *ctx);
void sessions_handler(struct evhttp_request *req, void *ctx);
void sessions_deleter(struct evhttp_request *req, void *ctx);
extern struct session_store global_sessions;


#endif // HTTP_H