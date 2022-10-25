#pragma once

#include "view.h"

// GET, POST /api/user
json_t *get_api_user_request(struct mg_str s);
json_t *post_api_user_request(struct mg_str s);
void api_user_view(struct mg_connection *, struct mg_http_message *, sqlite3 *);
