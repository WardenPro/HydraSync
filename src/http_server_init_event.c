#include "http.h"

route_entry routes[] = {
    {"/health", health_handler, EVHTTP_REQ_GET},
    {"/sessions", sessions_handler, EVHTTP_REQ_POST},
    {NULL, NULL, 0} // Terminator
};

static void signal_cb(evutil_socket_t fd, short event, void *arg) {
   printf("%s signal received\n", strsignal(fd));
   event_base_loopbreak(arg);
}

int http_server_init_event(const char *http_addr, int http_port) {
   struct event_base *base;
   struct evhttp *http_server;
   struct event *sig_int;
   base = event_base_new();
   http_server = evhttp_new(base);
   evhttp_bind_socket(http_server, http_addr, http_port);
   for (int i = 0; routes[i].path != NULL; i++) {
      evhttp_set_cb(http_server, routes[i].path, routes[i].handler,
                    &routes[i]); // Pass route info as context
   }
   evhttp_set_gencb(http_server, sessions_handler, NULL); 

   sig_int = evsignal_new(base, SIGINT, signal_cb, base);
   event_add(sig_int, NULL);

   printf("Listening requests on http://%s:%d\n", http_addr, http_port);

   event_base_dispatch(base);

   evhttp_free(http_server);
   event_free(sig_int);
   event_base_free(base);
   return 0;
}