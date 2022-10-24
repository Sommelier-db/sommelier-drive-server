#pragma once

#include <jansson.h>
#include <sqlite3.h>

#include "entity.h"
#include "mongoose.h"
#include "orm.h"
#include "router.h"
#include "util.h"

void main_view(struct mg_connection *, struct mg_http_message *, sqlite3 *);
void api_users_view(struct mg_connection *, struct mg_http_message *,
                    sqlite3 *);
