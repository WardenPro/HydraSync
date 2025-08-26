#include "http.h"

void sessions_handler(struct evhttp_request *req, void *ctx) {
   struct route_entry *route = (struct route_entry *)ctx;
   enum evhttp_cmd_type method = evhttp_request_get_command(req);
   
   // Check if the method is allowed for this route
   if (method != route->methods) {
       evhttp_send_error(req, 405, "Method Not Allowed");
       return;
   }
   
   struct evbuffer *reply = evbuffer_new();
   char uuid_str[37];
   struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
   uuid_t session_uuid;
   uuid_generate(session_uuid);
   uuid_unparse(session_uuid, uuid_str);
   evbuffer_add_printf(reply, "%s", uuid_str);
   evhttp_add_header(headers, "Content-Type", "application/json");
   evhttp_add_header(headers, "Session_uuid", uuid_str);

   evhttp_send_reply(req, HTTP_OK, NULL, reply);
   evbuffer_free(reply);
}