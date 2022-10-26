#pragma once

#include "view.h"

// GET, POST /api/write-permission
json_t *get_api_write_permission_request(struct mg_str s);
json_t *post_api_write_permission_request(struct mg_str s);
void api_write_permission_view(struct mg_connection *, struct mg_http_message *,
                               SommelierDBMS *);
