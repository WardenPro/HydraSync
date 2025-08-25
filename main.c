#include "http.h"

int main(int argc, char **argv)
{
    //Option Handler

    char *http_addr = "0.0.0.0";
    ev_uint16_t http_port = 8080;

    http_server_init_event(http_addr, http_port);
}