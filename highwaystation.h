#ifndef HS
#define HS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct highway_station {
    int distance;
    int nCars;
    int maxcarautonomy;
    struct highway_station *prec;
    struct highway_station *left;
    struct highway_station *right;
    struct car *root;
} highway_station;


typedef struct car{
    int autonomy;
    int n;
    struct car *prec;
    struct car *left;
    struct car *right;
} car;

void aggiungistazione(highway_station **highway, int distanza, int nAuto, int **autonomia);
void aggiungiauto(highway_station **highway, int distanza, int autonomia);
void demoliscistazione(highway_station **highway, int distance);
void rottamauto(highway_station **highway, int distance, int autonomy);
int pianificapercorso(highway_station **highway, int partenza, int arrivo, int**path);

#endif