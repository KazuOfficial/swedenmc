#include "RecipeSorter.h"
#include "ShapedRecipes.h"
#include "ShapelessRecipes.h"
#include <stdlib.h>

RecipeSorter *RecipeSorter_create(CraftingManager *var1) {
    RecipeSorter *self = (RecipeSorter *)calloc(1, sizeof(RecipeSorter));
    self->craftingManager = var1;
    return self;
}

int RecipeSorter_compareRecipes(RecipeSorter *self, IRecipe *var1, IRecipe *var2) {
    (void)self;
    if (ShapelessRecipes_isInstance(var1) && ShapedRecipes_isInstance(var2))
        return 1;
    if (ShapelessRecipes_isInstance(var2) && ShapedRecipes_isInstance(var1))
        return -1;
    int s1 = var1->vtable->getRecipeSize(var1);
    int s2 = var2->vtable->getRecipeSize(var2);
    return s2 < s1 ? -1 : (s2 > s1 ? 1 : 0);
}

int RecipeSorter_compare(RecipeSorter *self, void *var1, void *var2) {
    return RecipeSorter_compareRecipes(self, (IRecipe *)var1, (IRecipe *)var2);
}
