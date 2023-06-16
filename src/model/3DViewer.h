#ifndef SRC_MODEL_3DVIEWER_H_
#define SRC_MODEL_3DVIEWER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct polygon {
    // f 0 1 3 2
    int *P; // значения после f
    int amount_V; // их кол-во
} polygon;

typedef struct vertex {
    // v 1 2 3 4
    double x;
    double y;
    double z;
} vertex;


typedef struct viewer {
    vertex *V;
    polygon *P;

    // кол-во V и P
    int amount_V;
    int amount_P;

    // max и min точки
    double X_max;
    double Y_max;
    double Z_max;
    double X_min;
    double Y_min;
    double Z_min;
} viewer;


int f_parser(char *file_model, viewer *Data);
void f_clean(viewer *Data);
int f_countvf(FILE *file, viewer *Data);
int f_calloc(viewer *Data);
int f_free(viewer *Data);
int f_getstruct(FILE *file, viewer *Data);
int f_check(char *str_f);
void f_getV(char *str_f, viewer *Data, int *num);
void f_getP(char *str_f, viewer *Data, int *num);
//void f_center(viewer *Data);
void f_printf(viewer *Data);



// // подводим под заданную шкалу, иначе под шкалу -1...1
// int f_scale(viewer *Data, ...);

// // поворот фигуры
// int f_turnX(viewer *Data, ...);
// int f_turnY(viewer *Data, ...);
// int f_turnZ(viewer *Data, ...);

#endif  // SRC_MODEL_3DVIEWER_H_
