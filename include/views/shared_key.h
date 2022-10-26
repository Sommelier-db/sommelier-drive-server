#pragma once

#include "view.h"

// GET, POST /api/shared-key
json_t *get_api_shared_key_request(struct mg_str s);
json_t *post_api_shared_key_request(struct mg_str s);
void api_shared_key_view(struct mg_connection *, struct mg_http_message *,
                         SommelierDBMS *);
