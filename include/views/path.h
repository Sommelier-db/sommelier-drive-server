#pragma once

#include "view.h"

// GET, POST /api/file-path
json_t *get_api_path_request(struct mg_str s);
json_t *post_api_path_request(struct mg_str s);
void api_path_view(struct mg_connection *, struct mg_http_message *, sqlite3 *);

// GET /api/file-path/children
json_t *get_api_path_children_request(struct mg_str s);
void api_path_children_view(struct mg_connection *, struct mg_http_message *,
                            sqlite3 *);

// GET /api/file-path/search
json_t *get_api_path_search_request(struct mg_str s);
void api_path_search_view(struct mg_connection *, struct mg_http_message *,
                          sqlite3 *);
