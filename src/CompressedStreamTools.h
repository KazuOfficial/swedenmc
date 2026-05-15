#ifndef COMPRESSED_STREAM_TOOLS_H
#define COMPRESSED_STREAM_TOOLS_H

#include "NBTTagCompound.h"
#include "DataStream.h"
#include <stdio.h>

NBTTagCompound *CompressedStreamTools_func_1141_a(DataStream *in);

void CompressedStreamTools_func_1139_a(NBTTagCompound *tag, DataStream *out);

NBTTagCompound *CompressedStreamTools_func_1138_a(FILE *in);

void CompressedStreamTools_writeGzippedCompoundToOutputStream(NBTTagCompound *tag, FILE *out);

#endif
