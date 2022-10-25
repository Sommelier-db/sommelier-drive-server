#pragma once

#include "view.h"

// GET, POST /api/authorization-seed
json_t *get_api_authorization_seed_request(struct mg_str s);
json_t *post_api_authorization_seed_request(struct mg_str s);
void api_authorization_seed_view(struct mg_connection *,
                                 struct mg_http_message *, sqlite3 *);
