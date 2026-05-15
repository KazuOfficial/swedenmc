#include "OpenGlCapsChecker.h"
#include <GLFW/glfw3.h>

static int tryCheckOcclusionCapable = 1;

int OpenGlCapsChecker_checkARBOcclusion(OpenGlCapsChecker *self) {
    (void)self;

    return tryCheckOcclusionCapable && glfwExtensionSupported("GL_ARB_occlusion_query");
}
