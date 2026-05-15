#ifndef VEC3D_H
#define VEC3D_H

typedef struct {
    double xCoord;
    double yCoord;
    double zCoord;
} Vec3D;

void Vec3D_clearPool(void);
void Vec3D_initialize(void);

Vec3D *Vec3D_createVectorHelper(double x, double y, double z);
Vec3D *Vec3D_createVector(double x, double y, double z);

Vec3D *Vec3D_subtract(Vec3D *self, Vec3D *other);
Vec3D *Vec3D_normalize(Vec3D *self);
Vec3D *Vec3D_crossProduct(Vec3D *self, Vec3D *other);
Vec3D *Vec3D_addVector(Vec3D *self, double x, double y, double z);
double Vec3D_distanceTo(Vec3D *self, Vec3D *other);
double Vec3D_squareDistanceTo(Vec3D *self, Vec3D *other);
double Vec3D_squareDistanceTo3(Vec3D *self, double x, double y, double z);
double Vec3D_lengthVector(Vec3D *self);
Vec3D *Vec3D_getIntermediateWithXValue(Vec3D *self, Vec3D *other, double x);
Vec3D *Vec3D_getIntermediateWithYValue(Vec3D *self, Vec3D *other, double y);
Vec3D *Vec3D_getIntermediateWithZValue(Vec3D *self, Vec3D *other, double z);
void Vec3D_rotateAroundX(Vec3D *self, float angle);
void Vec3D_rotateAroundY(Vec3D *self, float angle);
const char *Vec3D_toString(Vec3D *self);

#endif
