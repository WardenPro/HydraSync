#include "http.h"

void health_handler(struct evhttp_request *req, void *ctx)
{
    struct route_entry *route = (struct route_entry *)ctx;
    enum evhttp_cmd_type method = evhttp_request_get_command(req);
    
    // Check if the method is allowed for this route
    if (method != route->methods) {
        evhttp_send_error(req, 405, "Method Not Allowed");
        return;
    }
    
    struct evbuffer *reply = evbuffer_new();

    evbuffer_add_printf(reply, "Ok");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    evbuffer_free(reply);
}