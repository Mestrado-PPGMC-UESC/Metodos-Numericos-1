/* secante.h */
#ifndef SECANTE_H
#define SECANTE_H

double f(double x);
double secante(double x0, double x1, double tol_percent, int max_iter, int verbose);

#endif
