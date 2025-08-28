#include "http.h"

void sessions_handler(struct evhttp_request *req, void *ctx) {
   route_entry *route = (struct route_entry *)ctx;
   enum evhttp_cmd_type methods = evhttp_request_get_command(req);
   const char *uri = evhttp_request_get_uri(req);
   
   printf("DEBUG: sessions_handler appelé avec URI: %s\n", uri);

   if (strncmp(uri, "/sessions/", 10) == 0 && strlen(uri) > 10) {
      const char *uuid_str = uri + 10; // Skip "/sessions/"
      printf("test - URI avec UUID detectee\n");
      if (strlen(uuid_str) == 36 
      && uuid_str[8] == '-' && uuid_str[13] == '-' 
      && uuid_str[18] == '-' && uuid_str[23] == '-') {
         printf("test - UUID valide detecte\n");
         if (methods == EVHTTP_REQ_DELETE) {
            sessions_deleter(req, (void *)uuid_str);
            return;
         } else if (methods == EVHTTP_REQ_POST) {
            printf("test - POST sur UUID\n");
            //sessions_source(req, uuid_str);
            return;
         } else {
            evhttp_send_error(req, 405, "Method Not Allowed");
            return;
         }
      } else {
         evhttp_send_error(req, 400, "Invalid UUID format");
         return;
      }
   }

   // Handle /sessions (collection) requests
   if (strcmp(uri, "/sessions") != 0) {
      evhttp_send_error(req, 404, "Not Found");
      return;
   }

   // Check if the method is allowed for this route (only for /sessions, not for gencb)
   if (route != NULL && methods != route->methods) {
      evhttp_send_error(req, 405, "Method Not Allowed");
      return;
   }
   struct evbuffer *reply = evbuffer_new();
   struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
   
   // Allocate memory if needed
   if (global_sessions.sessions == NULL) {
      global_sessions.sessions = malloc(global_sessions.capacity * sizeof(session));
      if (global_sessions.sessions == NULL) {
         evhttp_send_error(req, 500, "Internal Server Error");
         evbuffer_free(reply);
         return;
      }
   }
   
   // Reallocate if at capacity
   if (global_sessions.count >= global_sessions.capacity) {
      global_sessions.capacity *= 2;
      global_sessions.sessions = realloc(global_sessions.sessions, 
                                       global_sessions.capacity * sizeof(session));
      if (global_sessions.sessions == NULL) {
         evhttp_send_error(req, 500, "Internal Server Error");
         evbuffer_free(reply);
         return;
      }
   }
   
   session *new_session = &global_sessions.sessions[global_sessions.count];
   uuid_t temp_uuid;
   uuid_generate(temp_uuid);
   uuid_unparse(temp_uuid, new_session->session_token);
   
   global_sessions.count += 1;
   
   evbuffer_add_printf(reply, "%s", new_session->session_token);
   evhttp_add_header(headers, "Content-Type", "application/json");
   evhttp_add_header(headers, "Session_uuid", new_session->session_token);
   evhttp_send_reply(req, HTTP_OK, NULL, reply);
   evbuffer_free(reply);
}