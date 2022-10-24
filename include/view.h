#pragma once

#include <jansson.h>
#include <sqlite3.h>

#include "entity.h"
#include "mongoose.h"
#include "orm.h"
#include "router.h"
#include "util.h"
#include "views/user.h"

#define __ERROR_REPLY(c) \
    (mg_http_reply(c, 500, "", "{\"msg\": \"internal error\"}\n"))

char *request_method(struct mg_str);
int json_has_key(json_t *, const char *, int);

// GET, POST /api/file-path
json_t *get_api_path_request(struct mg_str s);
json_t *post_api_path_request(struct mg_str s);
void api_path_view(struct mg_connection *, struct mg_http_message *, sqlite3 *);

// GET /api/file-path/children

// GET /api/file-path/search

// GET, POST /api/shared-key

// GET, POST /api/authorization-seed

// GET, POST, PUT /api/content

// GET, POST /api/write-permission
