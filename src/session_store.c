#include "http.h"

struct session_store global_sessions = {
    .sessions = NULL,
    .count = 0,
    .capacity = 5
};

