#include "http.h"

void sessions_handler(struct evhttp_request *req, void *ctx)
{
        struct evbuffer *reply = evbuffer_new();

        evbuffer_add_printf(reply, "Ok session");
        evhttp_send_reply(req, HTTP_OK, NULL, reply);
        evbuffer_free(reply);
        
}