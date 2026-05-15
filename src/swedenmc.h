#pragma once

typedef struct {
    char username[64];
    char uuid[64];
    char mc_token[2048];
    char assets_dir[1024];
    char asset_index_path[1024];
} SwedenMCSession;

int swedenmc_show_launcher(SwedenMCSession *session_out);
