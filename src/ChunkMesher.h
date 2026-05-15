#ifndef CHUNK_MESHER_H
#define CHUNK_MESHER_H

#include "WorldRenderer.h"

void ChunkMesher_init(int numThreads);

void ChunkMesher_shutdown(void);

void ChunkMesher_drain(void);

void ChunkMesher_submit(WorldRenderer *wr);

void ChunkMesher_cancel(WorldRenderer *wr);

int ChunkMesher_flush(void);

#endif
