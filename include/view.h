#pragma once

#include <jansson.h>
#include <sqlite3.h>

#include "entity.h"
#include "mongoose.h"
#include "orm.h"
#include "router.h"
#include "util.h"
#include "views/authorization_seed.h"
#include "views/content.h"
#include "views/path.h"
#include "views/shared_key.h"
#include "views/user.h"

#define __ERROR_REPLY(c) \
    (mg_http_reply(c, 500, "", "{\"msg\": \"internal error\"}\n"))

char *request_method(struct mg_str);
int json_has_key(json_t *, const char *, int);
