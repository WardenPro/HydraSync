#include "http.h"

void sessions_handler(struct evhttp_request *req, void *ctx) {
   struct evbuffer *reply = evbuffer_new();
   char uuid_str[37];
   struct evkeyvalq *headers = evhttp_request_get_output_headers(req);

   uuid_unparse(ctx, uuid_str);
   evbuffer_add_printf(reply, uuid_str);
   evhttp_add_header(headers, "Content-Type", "application/json");
   evhttp_add_header(headers, "Session_uuid", uuid_str);

   evhttp_send_reply(req, HTTP_OK, NULL, reply);
   evbuffer_free(reply);
}