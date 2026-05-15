#include "JavaMath.h"
#include "JavaRandom.h"

static JavaRandom math_randomNumberGenerator;
static int math_randomNumberGenerator_inited = 0;

double Math_random(void) {
    if (!math_randomNumberGenerator_inited) {
        JavaRandom_new(&math_randomNumberGenerator);
        math_randomNumberGenerator_inited = 1;
    }
    return JavaRandom_nextDouble(&math_randomNumberGenerator);
}
