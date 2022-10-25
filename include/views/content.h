#pragma once

#include "view.h"

// GET, POST, PUT /api/content
json_t *get_api_content_request(struct mg_str s);
json_t *post_api_content_request(struct mg_str s);
json_t *put_api_content_request(struct mg_str s);
void api_content_view(struct mg_connection *, struct mg_http_message *,
                      sqlite3 *);
