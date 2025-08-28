#include "http.h"

void sessions_deleter(struct evhttp_request *req, void *ctx) {
   enum evhttp_cmd_type methods = evhttp_request_get_command(req);
   
   struct evbuffer *reply = evbuffer_new();
   struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
   const char *uuid_str = (const char *)ctx;
   if (uuid_str == NULL) {
      evhttp_send_error(req, 400, "Bad Request");
      return;
   } else if (uuid_str != NULL) {
      for (size_t i = 0; i < global_sessions.count; i++) {
         if (strcmp(global_sessions.sessions[i].session_token, uuid_str) == 0) {
            strcpy(global_sessions.sessions[i].session_token, 
                   global_sessions.sessions[global_sessions.count - 1].session_token);
            global_sessions.count -= 1;
            evhttp_remove_header(headers, "Session_uuid");
            break;
         }
      }
   }

   evhttp_send_reply(req, HTTP_OK, NULL, reply);
   evbuffer_free(reply);
}