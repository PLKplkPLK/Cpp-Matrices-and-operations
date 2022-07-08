#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


// zwraca: 1 - udane, 0 - nieudane
int CreateMatrix( double*** pTab, int nDim );

//rozwiniecie wzgl 0-go wiersza wyznacznik rekurencyjnie 
// dla 2x2 uzyc znanego wzoru - mozna zaindeksowac
double Det( double** pTab, int nDim );

// *pTab=NULL
void DeleteMatrix( double*** pTab, int nDim );

//transponowanie macierzy
// bez pomocniczej, raz wolno indeksowac
void TransMatrix( double** pTab, int nDim ); 

// macierz odwrotna, na we obliczony wczesniej w main() det  (tu w funkcji juz na pewno musi byc det!=0)
void InverseMatrix( double** pInv, double** pTab, int nDim, double det );

// rozwiazuje ukl rownan
void LayoutEqu( double** pInv, double* pB, double* pRes, int nDim );

// drukuje macierz 
void PrintMatrix( double** pTab, int nDim );

#endif