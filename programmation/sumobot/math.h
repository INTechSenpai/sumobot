/*
	Tout ceci n'est que de la poudre aux yeux pour VisualStudio, afin qu'il arrête de tout souligner en rouge pour rien !
*/

#ifndef __MATH_H
#define __MATH_H

#ifndef M_PI
#define M_PI        3.14159265358979323846  /* pi */
#endif // !M_PI

#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923  /* pi/2 */
#endif // !M_PI_2

#ifndef M_PI_4
#define M_PI_4      0.78539816339744830962  /* pi/4 */
#endif // !M_PI_4


extern int strcmp(const char*, const char*);

#ifdef __cplusplus
extern "C" {
#endif

extern double cos(double __x) ;
extern double fabs(double __x) ;
extern double fmod(double __x, double __y) ;
extern double modf(double __x, double *__iptr);
extern double sin(double __x) ;
extern double sqrt(double __x)  ;
extern double tan(double __x) ;
extern double floor(double __x) ;
extern double ceil(double __x) ;
extern double frexp(double __x, int *__pexp);
extern double ldexp(double __x, int __exp) ;
extern double exp(double __x) ;
extern double cosh(double __x) ;
extern double sinh(double __x) ;
extern double tanh(double __x) ;
extern double acos(double __x) ;
extern double asin(double __x) ;
extern double atan(double __x) ;
extern double atan2(double __y, double __x) ;
extern double log(double __x) ;
extern double log10(double __x) ;
extern double pow(double __x, double __y) ;
extern double fdim (double __x, double __y) ;
extern double fma (double __x, double __y, double __z) ;
extern double fmax (double __x, double __y) ;
extern double fmin (double __x, double __y) ;
extern double trunc(double __x);

#ifdef __cplusplus
}
#endif

#endif /* _MATH_H */