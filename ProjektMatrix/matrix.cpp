#include "matrix.h"

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim );
void ComplMatrix( double** pTabD, double** pTab, int nDim );

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim )
{
	// wycina wiersz i kolumne z tablicy wejsciowej pTabI i reszte kopiuje do tablicy wyjsciowej pTabO
	for( int i = 0; i < nDim; i++, pTabI++ )
	{
		double* pi = *pTabI;
		if( i == nRow ) continue;
		double* po = *pTabO++;
		for( int j = 0; j < nDim; j++, pi++ )
		{
			if( j == nCol ) continue;
			*po++ = *pi;
		}
	}
}

void ComplMatrix( double** pTabD, double** pTab, int nDim )
{
	// oblicza macierz dopelnien pTabD na podstawie tablicy wejsciowej pTab
	double** MatTemp = NULL;
	if( !CreateMatrix( &MatTemp, nDim - 1 ) )
	{
		perror( "Error creating matrix in ComplMatrix." );
		return;
	}
	for( int i = 0; i < nDim; i++ )
	{
		double* p = *pTabD++;
		int k = ( i % 2 )? -1 : 1;
		for( int j = 0; j < nDim; j++ )
		{
			Complement( MatTemp, pTab, i, j, nDim );
			*p++ = k * Det( MatTemp, nDim - 1 );
			k = -k;
		}
	}
	DeleteMatrix( &MatTemp, nDim - 1 );
}

int CreateMatrix( double*** pTab, int nDim ) //zwraca 1 - ok, 0 - b³¹d
{
	// alokacja tablicy double* (1D) wskazników
	*pTab = (double**)malloc( nDim * sizeof(double*) );
	if( !*pTab ) return 0;
	memset( *pTab, 0, nDim * sizeof(double*) );
	// alokacja tablicy double (1D) dla poszczególnych wskazników
	double** pp = *pTab;
	for( int i = 0; i < nDim; i++ )
	{
		*pp = (double*)malloc( nDim * sizeof(double) );
		if( !*pp ) return 0;
		memset( *pp++, 0, nDim * sizeof(double) );
	}
	return 1;
}

double Det( double** pTab, int nDim )
{
	// wyznacznik rekurencyjnie, rozwiniecie wzgl 0-go wiersza 
	if( nDim == 1 ) return **pTab;
	if( nDim == 2 ) return ( **pTab * pTab[1][1] ) - ( pTab[0][1] * pTab[1][0] );
	double** TabCom = NULL;
	if( !CreateMatrix( &TabCom, nDim - 1 ) )
	{
		perror( "Error creating matrix in Det" );
		return 0;
	}
	double det = 0;
	double* p = *pTab;
	int z = 1;
	for( int i = 0; i < nDim; i++ )
	{
		Complement( TabCom, pTab, 0, i, nDim );
		det += z * ( *p++ ) * Det( TabCom, nDim - 1 );
		z = -z;
	}
	DeleteMatrix( &TabCom, nDim - 1 );
	return det;
}


void TransMatrix( double** pTab, int nDim )
{
	double** pTabC = pTab;
	for( int i = 0; i < nDim - 1; i++ )
	{
		double* ptr = *pTabC++ + i + 1;
		for( int j = i + 1; j < nDim; j++ )
		{
			double temp = *ptr;
			*ptr++ = pTab[j][i];
			pTab[j][i] = temp;
		}
	}
}

void DeleteMatrix( double*** pTab, int nDim )
{
	// usuwa macierz
	double** p = *pTab;
	for( int i = 0; i < nDim; i++ ) free( *p++ );
	free( *pTab );
	*pTab = NULL;
}

void InverseMatrix( double** pInv, double** pTab, int nDim, double det )
{
	// oblicza macierz odwrotna pInv, na we obliczony wczesniej w main() det
	ComplMatrix( pInv, pTab, nDim );
	TransMatrix( pInv, nDim );
	for( int i = 0; i < nDim; i++ )
	{
		double* p = *pInv++;
		for( int j = 0; j < nDim; j++ ) *p++ /= det;
	}
}

void LayoutEqu( double** pInv, double* pB, double* pRes, int nDim )
{
	// oblicza wektor wynikowy pRes
	for( int i = 0; i < nDim; i++, pRes++ )
	{
		double* p = *pInv++;
		double* pBt = pB;
		for( int j = 0; j < nDim; j++ )
		{
			*pRes += *p++ * *pBt++;
		}
	}
}

void PrintMatrix( double** pTab, int nDim )
{
	// print macierz
	for( int i = 0; i < nDim; i++ )
	{
		double* p = *pTab++;
		for( int j = 0; j < nDim; j++ ) printf( "%.2f ", *p++ );
		printf( "\n " );
	}
}