.DEFAULT_GOAL := all

CC      = clang
STD     = -std=c99
WARN    = -Wall -Wextra -Wpedantic \
          -Wno-typedef-redefinition \
          -Wno-gnu-statement-expression \
          -Wno-gnu-empty-struct \
          -Wno-empty-translation-unit
OPT_REL = -O3 -flto=thin -fomit-frame-pointer \
          -fno-stack-protector -DNDEBUG -funroll-loops
OPT_DBG = -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
OPT_LEAKS = -g -O0 -fno-omit-frame-pointer
OPT     = $(OPT_REL)
STRIP_RELEASE = 1

POSIX_FLAGS =
CFLAGS  = $(STD) $(WARN) $(OPT) $(POSIX_FLAGS)

SRC_DIR  = src
LIB_DIR  = lib
BUILD_DIR = build
BIN_DIR  = bin

UNAME   := $(shell uname -s)
MACHINE := $(shell uname -m)

BGFX_INCLUDES = \
	-I$(LIB_DIR)/bgfx/include \
	-I$(LIB_DIR)/bx/include \
	-I$(LIB_DIR)/bimg/include

ifeq ($(UNAME), Darwin)

TARGET = $(BIN_DIR)/swedenmc

ifeq ($(MACHINE), arm64)
BGFX_GCC_TARGET = osx-arm64
BGFX_BIN_DIR    = $(LIB_DIR)/bgfx/.build/osx-arm64/bin
OPT_REL = -O3 -march=armv8-a -flto=thin -fomit-frame-pointer \
          -fno-stack-protector -DNDEBUG -funroll-loops
else
BGFX_GCC_TARGET = osx-x64
BGFX_BIN_DIR    = $(LIB_DIR)/bgfx/.build/osx-x64/bin
OPT_REL = -O3 -march=x86-64 -flto=thin -fomit-frame-pointer \
          -fno-stack-protector -DNDEBUG -funroll-loops
endif

GENIE         = $(LIB_DIR)/bx/tools/bin/darwin/genie
BGFX_PROJ_DIR = $(LIB_DIR)/bgfx/.build/projects/gmake-$(BGFX_GCC_TARGET)
BGFX_LIBS     = \
	$(BGFX_BIN_DIR)/libbgfxRelease.a \
	$(BGFX_BIN_DIR)/libbxRelease.a \
	$(BGFX_BIN_DIR)/libbimgRelease.a \
	$(BGFX_BIN_DIR)/libbimg_decodeRelease.a
SHADERC       = $(BGFX_BIN_DIR)/shadercRelease

BGFX_INCLUDES += -I$(LIB_DIR)/bx/include/compat/osx

INCLUDES = \
	-I$(LIB_DIR)/glad/include \
	-I$(LIB_DIR)/glfw/include \
	-I$(LIB_DIR)/openal-soft/include \
	-I$(LIB_DIR) \
	-I/opt/homebrew/include

LDFLAGS = \
	-L$(LIB_DIR)/glfw/build/src \
	-lglfw3 \
	-L$(LIB_DIR)/openal-soft/build \
	-lopenal \
	-framework OpenGL \
	-framework CoreAudio \
	-framework AudioToolbox \
	-framework Cocoa \
	-framework IOKit \
	-framework CoreVideo \
	-framework QuartzCore \
	-framework Metal \
	-framework MetalKit \
	$(shell brew --prefix minizip 2>/dev/null)/lib/libminizip.a \
	-lz \
	-lm \
	-lcurl \
	-lc++ \
	-Wl,-stack_size,0x4000000

GLFW_LIB          = $(LIB_DIR)/glfw/build/src/libglfw3.a
OPENAL_LIB        = $(LIB_DIR)/openal-soft/build/libopenal.a
OPENAL_CMAKE_EXTRA = -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0

else ifeq ($(UNAME), Linux)

POSIX_FLAGS = -D_GNU_SOURCE

ifeq ($(MACHINE), aarch64)
OPT_REL = -O3 -march=armv8-a -fomit-frame-pointer \
          -fno-stack-protector -DNDEBUG -funroll-loops
else
OPT_REL = -O3 -march=x86-64 -fomit-frame-pointer \
          -fno-stack-protector -DNDEBUG -funroll-loops
endif

TARGET = $(BIN_DIR)/swedenmc

GENIE           = $(LIB_DIR)/bx/tools/bin/linux/genie
BGFX_GCC_TARGET = linux-clang
BGFX_PROJ_DIR   = $(LIB_DIR)/bgfx/.build/projects/gmake-linux-clang
BGFX_BIN_DIR    = $(LIB_DIR)/bgfx/.build/linux64_clang/bin
BGFX_LIBS       = \
	$(BGFX_BIN_DIR)/libbgfxRelease.a \
	$(BGFX_BIN_DIR)/libbxRelease.a \
	$(BGFX_BIN_DIR)/libbimgRelease.a \
	$(BGFX_BIN_DIR)/libbimg_decodeRelease.a
SHADERC         = $(BGFX_BIN_DIR)/shadercRelease

INCLUDES = \
	-I$(LIB_DIR)/glad/include \
	-I$(LIB_DIR)/glfw/include \
	-I$(LIB_DIR)/openal-soft/include \
	-I$(LIB_DIR)

LDFLAGS = \
	-L$(LIB_DIR)/glfw/build/src \
	-lglfw3 \
	-L$(LIB_DIR)/openal-soft/build \
	-lopenal \
	-lGL \
	-lX11 -lXrandr -lXi -lXinerama -lXcursor \
	-Wl,-Bstatic \
	-lminizip \
	-Wl,-Bdynamic \
	-lz \
	-lm \
	-lcurl \
	-lcrypto \
	-lstdc++ \
	-lpthread -ldl

GLFW_LIB   = $(LIB_DIR)/glfw/build/src/libglfw3.a
OPENAL_LIB = $(LIB_DIR)/openal-soft/build/libopenal.a

else  # Windows (MSYS2/MinGW)

POSIX_FLAGS = -D_POSIX_C_SOURCE=200809L -DAL_LIBTYPE_STATIC -DCURL_STATICLIB
ifeq ($(MACHINE), aarch64)
OPT_REL = -O3 -march=armv8-a -fomit-frame-pointer -fno-stack-protector -DNDEBUG -funroll-loops
else
OPT_REL = -O3 -march=x86-64 -fomit-frame-pointer -fno-stack-protector -DNDEBUG -funroll-loops
endif

TARGET = $(BIN_DIR)/swedenmc.exe

BGFX_MAKE_EXTRA    = AR=llvm-ar
GLFW_CMAKE_EXTRA   = -DGLFW_BUILD_WAYLAND=OFF -G "MinGW Makefiles"
SHADERS_HLSL       = \
	$(SHADER_DIR)/vs_game_hlsl.h \
	$(SHADER_DIR)/fs_game_hlsl.h \
	$(SHADER_DIR)/fs_game_po_hlsl.h
OPENAL_CMAKE_EXTRA = -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS=-Wno-function-effects -DHAVE_WFUNCTION_EFFECTS=OFF -DLIBTYPE=STATIC -G "MinGW Makefiles"

GENIE           = $(LIB_DIR)/bx/tools/bin/windows/genie.exe
BGFX_GCC_TARGET = mingw-clang
BGFX_PROJ_DIR   = $(LIB_DIR)/bgfx/.build/projects/gmake-mingw-clang
BGFX_BIN_DIR    = $(LIB_DIR)/bgfx/.build/win64_mingw-clang/bin
BGFX_LIBS       = \
	$(BGFX_BIN_DIR)/libbgfxRelease.a \
	$(BGFX_BIN_DIR)/libbxRelease.a \
	$(BGFX_BIN_DIR)/libbimgRelease.a \
	$(BGFX_BIN_DIR)/libbimg_decodeRelease.a
SHADERC         = $(BGFX_BIN_DIR)/shadercRelease.exe

INCLUDES = \
	-I$(LIB_DIR)/glad/include \
	-I$(LIB_DIR)/glfw/include \
	-I$(LIB_DIR)/openal-soft/include \
	-I$(LIB_DIR)

CURL_LDFLAGS  := $(shell pkg-config --static --libs libcurl 2>/dev/null || echo -lcurl -lssl -lcrypto -lz)
MZIP_LDFLAGS  := $(shell pkg-config --static --libs minizip 2>/dev/null || echo -lminizip -lz)

LDFLAGS = \
	-L$(LIB_DIR)/glfw/build/src \
	-lglfw3 \
	-L$(LIB_DIR)/openal-soft/build \
	-lOpenAL32 \
	-lopengl32 \
	-lgdi32 -luser32 -lshell32 -lwinmm -lole32 -luuid \
	-lavrt \
	-lpsapi \
	-lm \
	-lstdc++ \
	-Wl,-Bstatic \
	-lpthread \
	$(CURL_LDFLAGS) \
	$(MZIP_LDFLAGS) \
	-Wl,-Bdynamic \
	-Wl,--subsystem,windows \
	-Wl,--stack,67108864

GLFW_LIB   = $(LIB_DIR)/glfw/build/src/libglfw3.a
OPENAL_LIB = $(LIB_DIR)/openal-soft/build/libOpenAL32.a

endif

SRCS  = $(shell find $(SRC_DIR) -name '*.c')
SRCS += $(LIB_DIR)/glad/src/gl.c
OBJS  = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

ifeq ($(UNAME), Darwin)
SRCS_M = $(shell find $(SRC_DIR) -name '*.m')
OBJS  += $(patsubst %.m, $(BUILD_DIR)/%.o, $(SRCS_M))
endif

$(BGFX_LIBS):
	cd $(LIB_DIR)/bgfx && ../../$(GENIE) --with-tools --gcc=$(BGFX_GCC_TARGET) gmake
	$(MAKE) -C $(BGFX_PROJ_DIR) config=release64 $(BGFX_MAKE_EXTRA) bgfx bx bimg bimg_decode

$(SHADERC): $(BGFX_LIBS)
	$(MAKE) -C $(BGFX_PROJ_DIR) config=release64 $(BGFX_MAKE_EXTRA) shaderc

SHADER_DIR   = src/shaders
SHADER_INC   = $(LIB_DIR)/bgfx/src

SHADERS_EMBEDDED = \
	$(SHADER_DIR)/vs_game_mtl.h \
	$(SHADER_DIR)/fs_game_mtl.h \
	$(SHADER_DIR)/fs_game_po_mtl.h \
	$(SHADER_DIR)/vs_game_glsl.h \
	$(SHADER_DIR)/fs_game_glsl.h \
	$(SHADER_DIR)/fs_game_po_glsl.h \
	$(SHADER_DIR)/vs_game_spv.h \
	$(SHADER_DIR)/fs_game_spv.h \
	$(SHADER_DIR)/fs_game_po_spv.h \
	$(SHADERS_HLSL)

$(SHADER_DIR)/%_mtl.h: $(SHADER_DIR)/%.sc $(SHADERC)
	$(SHADERC) -f $< -o $@ --type $(if $(findstring vs_,$<),v,f) \
		--platform osx -p metal --bin2c $(notdir $(basename $@)) \
		--varyingdef $(SHADER_DIR)/varying.def.sc -i $(SHADER_INC)

$(SHADER_DIR)/%_glsl.h: $(SHADER_DIR)/%.sc $(SHADERC)
	$(SHADERC) -f $< -o $@ --type $(if $(findstring vs_,$<),v,f) \
		--platform linux -p 120 --bin2c $(notdir $(basename $@)) \
		--varyingdef $(SHADER_DIR)/varying.def.sc -i $(SHADER_INC)

$(SHADER_DIR)/%_spv.h: $(SHADER_DIR)/%.sc $(SHADERC)
	$(SHADERC) -f $< -o $@ --type $(if $(findstring vs_,$<),v,f) \
		--platform linux -p spirv --bin2c $(notdir $(basename $@)) \
		--varyingdef $(SHADER_DIR)/varying.def.sc -i $(SHADER_INC)

$(SHADER_DIR)/%_hlsl.h: $(SHADER_DIR)/%.sc $(SHADERC)
	$(SHADERC) -f $< -o $@ --type $(if $(findstring vs_,$<),v,f) \
		--platform windows -p s_5_0 --bin2c $(notdir $(basename $@)) \
		--varyingdef $(SHADER_DIR)/varying.def.sc -i $(SHADER_INC)

$(GLFW_LIB):
	cmake -S $(LIB_DIR)/glfw -B $(LIB_DIR)/glfw/build \
		-DGLFW_BUILD_EXAMPLES=OFF \
		-DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_DOCS=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		$(GLFW_CMAKE_EXTRA)
	cmake --build $(LIB_DIR)/glfw/build --parallel 2

$(OPENAL_LIB):
	cmake -S $(LIB_DIR)/openal-soft -B $(LIB_DIR)/openal-soft/build \
		-DLIBTYPE=STATIC \
		-DALSOFT_EXAMPLES=OFF \
		-DALSOFT_TESTS=OFF \
		-DALSOFT_UTILS=OFF \
		-DALSOFT_INSTALL=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		$(OPENAL_CMAKE_EXTRA)
	cmake --build $(LIB_DIR)/openal-soft/build --parallel 2

.PHONY: all debug leaks clean clean-bgfx shaders deps run

deps: $(GLFW_LIB) $(OPENAL_LIB) $(BGFX_LIBS) $(SHADERC)

shaders: $(SHADERC) $(SHADERS_EMBEDDED)

all: $(GLFW_LIB) $(OPENAL_LIB) $(BGFX_LIBS) $(SHADERC) $(SHADERS_EMBEDDED) $(TARGET)

$(TARGET): $(OBJS) $(GLFW_LIB) $(OPENAL_LIB) $(BGFX_LIBS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OPT) $(OBJS) $(BGFX_LIBS) $(LDFLAGS) -o $@
	$(if $(STRIP_RELEASE),strip $@)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(BGFX_INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: %.m
	@mkdir -p $(dir $@)
	$(CC) $(OPT) $(INCLUDES) $(BGFX_INCLUDES) -c $< -o $@

$(BUILD_DIR)/src/RendererBGFX.o: $(SHADERS_EMBEDDED)

debug: OPT = $(OPT_DBG)
debug: STRIP_RELEASE =
debug: all
ifeq ($(UNAME), Darwin)
	dsymutil $(TARGET)
endif

leaks: OPT = $(OPT_LEAKS)
leaks: STRIP_RELEASE =
leaks: all
ifeq ($(UNAME), Darwin)
	dsymutil $(TARGET)
endif

run: all
	cd $(BIN_DIR) && ./swedenmc

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f $(SHADERS_EMBEDDED)

clean-bgfx:
	rm -rf $(LIB_DIR)/bgfx/.build

EMCC = emcc

BGFX_WASM_GCC_TARGET = wasm
BGFX_WASM_PROJ_DIR   = $(LIB_DIR)/bgfx/.build/projects/gmake-$(BGFX_WASM_GCC_TARGET)
BGFX_WASM_BIN_DIR    = $(LIB_DIR)/bgfx/.build/$(BGFX_WASM_GCC_TARGET)/bin
BGFX_WASM_LIBS       = \
	$(BGFX_WASM_BIN_DIR)/bgfxRelease.a \
	$(BGFX_WASM_BIN_DIR)/bxRelease.a \
	$(BGFX_WASM_BIN_DIR)/bimgRelease.a \
	$(BGFX_WASM_BIN_DIR)/bimg_decodeRelease.a

OPENAL_WASM_LIB = $(LIB_DIR)/openal-soft/build-wasm/libopenal.a

SHADERS_ESSL = \
	$(SHADER_DIR)/vs_game_essl.h \
	$(SHADER_DIR)/fs_game_essl.h \
	$(SHADER_DIR)/fs_game_po_essl.h

$(SHADER_DIR)/%_essl.h: $(SHADER_DIR)/%.sc $(SHADERC)
	$(SHADERC) -f $< -o $@ --type $(if $(findstring vs_,$<),v,f) \
		--platform asm.js -p 300_es --bin2c $(notdir $(basename $@)) \
		--varyingdef $(SHADER_DIR)/varying.def.sc -i $(SHADER_INC)

$(BGFX_WASM_LIBS):
	cd $(LIB_DIR)/bgfx && ../../$(GENIE) --gcc=$(BGFX_WASM_GCC_TARGET) gmake
	emmake $(MAKE) -C $(BGFX_WASM_PROJ_DIR) config=release bgfx bx bimg bimg_decode \
		CFLAGS="-pthread -fexceptions" CXXFLAGS="-pthread -fexceptions"

$(OPENAL_WASM_LIB):
	emcmake cmake -S $(LIB_DIR)/openal-soft -B $(LIB_DIR)/openal-soft/build-wasm \
		-DLIBTYPE=STATIC \
		-DALSOFT_EXAMPLES=OFF \
		-DALSOFT_TESTS=OFF \
		-DALSOFT_UTILS=OFF \
		-DALSOFT_INSTALL=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DALSOFT_CPUEXT_NEON=OFF \
		-DALSOFT_CPUEXT_SSE=OFF \
		-DALSOFT_CPUEXT_SSE2=OFF \
		-DALSOFT_CPUEXT_SSE3=OFF \
		-DALSOFT_CPUEXT_SSE4_1=OFF \
		-DHAVE_WFUNCTION_EFFECTS=OFF \
		-DCMAKE_C_FLAGS="-pthread -fexceptions" \
		-DCMAKE_CXX_FLAGS="-pthread -fexceptions"
	emmake cmake --build $(LIB_DIR)/openal-soft/build-wasm --parallel 2

WASM_EXCLUDE = \
	$(LIB_DIR)/glad/src/gl.c \
	$(SRC_DIR)/RendererGL.c \
	$(SRC_DIR)/swedenmc.c \
	$(SRC_DIR)/swedenmc_auth.c \
	$(SRC_DIR)/swedenmc_assets.c
WASM_SRCS = $(filter-out $(WASM_EXCLUDE), $(shell find $(SRC_DIR) -name '*.c'))
WASM_OBJS = $(patsubst %.c, $(BUILD_DIR)/wasm/%.o, $(WASM_SRCS))

WASM_INCLUDES = \
	-I$(LIB_DIR)/glfw/include \
	-I$(LIB_DIR)/openal-soft/include \
	-I$(LIB_DIR)

WASM_TARGET = $(BIN_DIR)/swedenmc.html

WASM_ASSET_DIRS  = achievement armor art environment font gui item lang misc mob null terrain title
WASM_ASSET_FILES = font.txt pack.png pack.txt particles.png terrain.png
WASM_PRELOAD = \
	$(foreach d,$(WASM_ASSET_DIRS),$(if $(wildcard $(BIN_DIR)/$(d)),--preload-file $(BIN_DIR)/$(d)@$(d),)) \
	$(foreach f,$(WASM_ASSET_FILES),$(if $(wildcard $(BIN_DIR)/$(f)),--preload-file $(BIN_DIR)/$(f)@$(f),))

$(BUILD_DIR)/wasm/%.o: %.c
	@mkdir -p $(dir $@)
	$(EMCC) $(STD) $(WARN) -O2 -D_GNU_SOURCE \
		$(WASM_INCLUDES) $(BGFX_INCLUDES) \
		-s USE_ZLIB=1 \
		-s USE_PTHREADS=1 \
		-fexceptions \
		-c $< -o $@

$(BUILD_DIR)/wasm/$(SRC_DIR)/RendererBGFX.o: $(SHADERS_EMBEDDED) $(SHADERS_ESSL)

.PHONY: wasm clean-wasm serve-wasm

wasm: $(BGFX_WASM_LIBS) $(OPENAL_WASM_LIB) $(SHADERC) $(SHADERS_EMBEDDED) $(SHADERS_ESSL) $(WASM_OBJS)
	@mkdir -p $(BIN_DIR)
	$(EMCC) -O2 $(WASM_OBJS) $(BGFX_WASM_LIBS) $(OPENAL_WASM_LIB) \
		-s USE_GLFW=3 \
		-s USE_WEBGL2=1 \
		-s FULL_ES3=1 \
		-s USE_ZLIB=1 \
		-s USE_PTHREADS=1 \
		-s PTHREAD_POOL_SIZE=32 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s INITIAL_MEMORY=134217728 \
		-s TOTAL_STACK=67108864 \
		-lm -lc++ \
		-fexceptions \
		-sASSERTIONS=1 \
		-sASYNCIFY \
		$(WASM_PRELOAD) \
		-o $(WASM_TARGET)

serve-wasm:
	cd $(BIN_DIR) && python3 ../serve.py

clean-wasm:
	rm -rf $(BUILD_DIR)/wasm $(WASM_TARGET) $(BIN_DIR)/swedenmc.js $(BIN_DIR)/swedenmc.wasm $(BIN_DIR)/swedenmc.data
	rm -f $(SHADERS_ESSL)
