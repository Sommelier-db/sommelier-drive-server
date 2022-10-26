#pragma once

#include <jansson.h>

#include "entity.h"
#include "mongoose.h"
#include "orm.h"
#include "router.h"
#include "sommelier-db.h"
#include "sommelier_drive_cryptos.h"
#include "util.h"
#include "views/authorization_seed.h"
#include "views/content.h"
#include "views/path.h"
#include "views/shared_key.h"
#include "views/user.h"
#include "views/write_permission.h"

#define __ERROR_REPLY(c) \
    (mg_http_reply(c, 500, "", "{\"msg\": \"internal error\"}\n"))

char *request_uri(struct mg_http_message *);
char *request_method(struct mg_http_message *);

int json_has_key(json_t *, const char *, int);

void logging_http_body(struct mg_http_message *hm);
