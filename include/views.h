#pragma once

#include "mongoose.h"
#include "router.h"
#include "util.h"

#define DEFINE_VIEW(__NAME__) \
    void __NAME__(struct mg_connection *, struct mg_http_message *, sqlite3 *)

DEFINE_VIEW(main_view);
DEFINE_VIEW(api_users_view);
