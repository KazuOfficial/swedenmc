#pragma once

typedef struct SwAssets SwAssets;

SwAssets *swassets_begin(void);
void swassets_free(SwAssets *a);

int swassets_poll(SwAssets *a);

float swassets_progress(SwAssets *a);
const char *swassets_status(SwAssets *a);
const char *swassets_error(SwAssets *a);

const char *swassets_dir(SwAssets *a);
const char *swassets_index_path(SwAssets *a);
