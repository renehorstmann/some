#ifndef U_POSE_H
#define U_POSE_H

#include <stdbool.h>
#include <math.h>
#include <limits.h>  // FLT_MAX
#include "mathc/mat/mat4.h"

// Pose matrix:
// Xx Yx Zx Tx
// Xy Yy Zy Ty
// Xz Yz Zz Tz
// 0  0  0  1

// Pose in form of eye will be at position 0, 0, with a size of 1, 1

// Example for 2D X,Y,W,W: 10, 20, 34, 44
// 17 22 0  10
// 17 22 0  20
// 0  0  1  0
// 0  0  0  1

// u_pose_aa_* are axis aligned (angle=0)

static mat4 u_pose_new(float x, float y, float w, float h) {
	// mat4 has column major order
	return (mat4) {{
		w, 0, 0, 0,
        0, h, 0, 0,
        0, 0, 1, 0,
        x, y, 0, 1
	}};
}

static mat4 u_pose_new_angle(float x, float y, float w, float h, float angle_rad) {
	// mat4 has column major order
	return (mat4) {{
		cosf(angle_rad) * w, sinf(angle_rad) * w, 0, 0,
        -sinf(angle_rad) * h, cosf(angle_rad) * h, 0, 0,
        0, 0, 1, 0,
        x, y, 0, 1
	}};
}

static mat4 u_pose_new_aa(float l, float t, float w, float h) {
	return u_pose_new(l+w/2, t-h/2, w, h);
}

static mat4 u_pose_new_hidden() {
	return u_pose_new(FLT_MAX, FLT_MAX, 1, 1);
}

static float u_pose_get_x(mat4 p) {
    return p.m30;
}

static float u_pose_get_y(mat4 p) {
    return p.m31;
}

static float u_pose_get_w(mat4 p) {
    return sqrtf(powf(p.m00, 2) + powf(p.m01, 2));
}

static float u_pose_get_h(mat4 p) {
    return sqrtf(powf(p.m10, 2) + powf(p.m11, 2));
}

static float u_pose_get_angle(mat4 p) {
    return atan2f(p.m01, p.m00);
}

static float u_pose_aa_get_left(mat4 p) {
    return p.m30 - u_pose_get_w(p) / 2;
}

static float u_pose_aa_get_right(mat4 p) {
    return p.m30 + u_pose_get_w(p) / 2;
}

static float u_pose_aa_get_top(mat4 p) {
    return p.m31 + u_pose_get_h(p) / 2;
}

static float u_pose_aa_get_bottom(mat4 p) {
    return p.m31 - u_pose_get_h(p) / 2;
}


static void u_pose_set_x(mat4 *p, float x) {
    p->m30 = x;
}

static void u_pose_set_y(mat4 *p, float y) {
    p->m31 = y;
}

static void u_pose_set_xy(mat4 *p, float x, float y) {
    u_pose_set_x(p, x);
    u_pose_set_y(p, y);
}

static void u_pose_set_hidden(mat4 *p) {
	u_pose_set_xy(p, FLT_MAX, FLT_MAX);
}

static void u_pose_set_size_angle(mat4 *p, float w, float h, float angle_rad) {
    p->m00 = cosf(angle_rad) * w;
    p->m01 = sinf(angle_rad) * w;
    p->m10 = -sinf(angle_rad) * h;
    p->m11 = cosf(angle_rad) * h;
}

static void u_pose_set_w(mat4 *p, float w) {
    float f = w / u_pose_get_w(*p);
    p->m00 *= f;
    p->m01 *= f;
}

static void u_pose_set_h(mat4 *p, float h) {
    float f = h / u_pose_get_h(*p);
    p->m10 *= f;
    p->m11 *= f;
}

static void u_pose_set_size(mat4 *p, float w, float h) {
    u_pose_set_w(p, w);
    u_pose_set_h(p, h);
}

static void u_pose_set_angle(mat4 *p, float angle_rad) {
    u_pose_set_size_angle(p,
                          u_pose_get_w(*p),
                          u_pose_get_h(*p),
                          angle_rad);
}

static void u_pose_set(mat4 *p, float x, float y, float w, float h, float angle_rad) {
    u_pose_set_xy(p, x, y);
    u_pose_set_size_angle(p, w, h, angle_rad);
}

static void u_pose_shift_x(mat4 *p, float x) {
    p->m30 += x;
}

static void u_pose_shift_y(mat4 *p, float y) {
    p->m31 += y;
}

static void u_pose_shift_xy(mat4 *p, float x, float y) {
    u_pose_shift_x(p, x);
    u_pose_shift_y(p, y);
}

static void u_pose_shift_angle(mat4 *p, float angle_rad) {
    u_pose_set_angle(p, u_pose_get_angle(*p) + angle_rad);
}

static void u_pose_shift(mat4 *p, float x, float y, float angle_rad) {
    u_pose_shift_xy(p, x, y);
    u_pose_shift_angle(p, angle_rad);
}


static void u_pose_aa_set_left(mat4 *p, float l) {
    p->m30 = l + u_pose_get_w(*p) / 2;
}

static void u_pose_aa_set_right(mat4 *p, float r) {
    p->m30 = r - u_pose_get_w(*p) / 2;
}

static void u_pose_aa_set_top(mat4 *p, float t) {
    p->m31 = t - u_pose_get_h(*p) / 2;
}

static void u_pose_aa_set_bottom(mat4 *p, float b) {
    p->m31 = b + u_pose_get_h(*p) / 2;
}

static void u_pose_aa_set(mat4 *p, float l, float t, float w, float h) {
	u_pose_set(p, l+w/2, t-w/2, w, h, 0);
}


static bool u_pose_contains(mat4 p, vec4 pos) {

    mat4 p_inv = mat4_inv(p);
    vec4 p_pos = mat4_mul_vec(p_inv, pos);

    return p_pos.x >= -0.5 && p_pos.x <= 0.5
           && p_pos.y >= -0.5 && p_pos.y <= 0.5;
}


static bool u_pose_aa_contains(mat4 p, vec2 pos) {
    float l = u_pose_aa_get_left(p);
    float r = u_pose_aa_get_right(p);
    float t = u_pose_aa_get_top(p);
    float b = u_pose_aa_get_bottom(p);

    return pos.x >= l && pos.x <= r && pos.y <= t && pos.y >= b;
}


#endif //U_POSE_H
