#ifndef UTILS_H
#define UTILS_H

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))
#define ABS(x) (((x) > 0) ? (x) : -(x))
#define constrain(x,y,z) ( ((x)<(y))?(y):( ((x)>(z))?(z):(x) ) )

#ifdef __cplusplus
extern "C" {
#endif

int modulo(int nombre, int modulo);

#ifdef __cplusplus
}
#endif

#endif