

#include <VEngine/Headers/vector.h>
#include <math.h>

Vec2 Vec2_set(float x, float y)
{
    Vec2 v = { .x = x, .y = y };
    return v;
}

Vec2 Vec2_add(Vec2 v1, Vec2 v2) {
    Vec2 result = { .x = v1.x + v2.x,
                    .y = v1.y + v2.y };
    return result;
}

Vec2 Vec2_add3(Vec2 v1, Vec2 v2, Vec2 v3) {
    return Vec2_add(Vec2_add(v1, v2), v3);
}

Vec2 Vec2_sub(Vec2 v1, Vec2 v2) {
    Vec2 result = { .x = v1.x - v2.x,
                    .y = v1.y - v2.y};
    return result;
}

Vec2 Vec2_scale(Vec2 v, float s) {
    Vec2 result = { .x = v.x * s,
                    .y = v.y *s};
    return result;
}

float Vec2_norm(Vec2 v) {

    return sqrt((v.x) * (v.x)
                + (v.y) * (v.y));
}

Vec2 Vec2_Negate(Vec2 v) {

    Vec2 result = { .x = -v.x,
                    .y = -v.y};

    return result;
}

Vec2 Vec2_normalize(Vec2 v) {

    float norm = Vec2_norm(v);

    Vec2 result = { .x = v.x / norm,
                    .y = v.y / norm };

    return result;

}

float Vec2_distance(Vec2 v1, Vec2 v2) {

    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));

}
