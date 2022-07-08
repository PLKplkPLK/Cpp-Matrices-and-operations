#define _DEBUG_

#include "matrix.h"

void ReadData( FILE* fin, double** pMatrix, double* b, int nDim );
int CreateVector( double**pv, int n );
void PrintVector( double* pv, int nDim );


int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		printf( "Error, usage: %s, <file with a matrix>\n", argv[0] );
		return 1;
	}

	FILE *fin = NULL;

	// otworzyc plik do odczytu (tekstowy)
	int nDim = 0;
	if( ( fin = fopen( argv[1], "r" ) ) == NULL )
	{
		perror( "Problem z otwarciem pliku\n" );
		return 2;
	}
	  // odczytac rozmiar nDim
	if( !fscanf( fin, "%i", &nDim ) )
	{
		printf( "Zle dane w pliku" );
		return 3;
	}

	// wydruk kontrolny
	#ifdef _DEBUG_
		printf( "\n nDim = %i, MAXDIM = %i, nDim powinno byc rowne MAXDIM\n", nDim, nDim );
	#endif

	// wykreowac macierz ukladu nDim x nDim
	double** pMatrix = NULL;
	if( !CreateMatrix( &pMatrix, nDim ) )
	{
		printf( "Blad w kreowaniu macierzy glownej." );
		return 4;
	}

	// wykreowac wektor wyrazow wolnych
	double* pB = NULL;
	if( !CreateVector( &pB, nDim ) )
	{
		perror( "Problem: nieudana alokacja *b" );
		return 5;
	}

	

	// wczytac dane ( ReadData() )
	ReadData( fin, pMatrix, pB, nDim );
	fclose( fin );

	// wydruk kontrolny
	#ifdef _DEBUG_
		printf( "\n Macierz odczytana:\n " );
		PrintMatrix( pMatrix, nDim );
		printf( "\n Wektor odczytany:" );
		PrintVector( pB, nDim );
	#endif

	// determinant
	double DetMain = Det( pMatrix, nDim );
	if( fabs(DetMain) < 1e-200 )
	{
		printf( "Wyznacznik macierzy rowny 0, nie mozna podac rozwiazan.\n" );
		return 6;
	}
	#ifdef _DEBUG_
		printf( "\n Wyznacznik = %.2f\n", DetMain );
	#endif

	// obrocic macierz
	double** pMatInv = NULL;
	if( !CreateMatrix( &pMatInv, nDim ) )
	{
		perror( "Blad w alokacji macierzy odwrotnej." );
		return 8;
	}
	InverseMatrix( pMatInv, pMatrix, nDim, DetMain );

	// wydruk kontrolny
	#ifdef _DEBUG_
		printf( "\n Macierz odwrotna:\n " );
		PrintMatrix( pMatInv, nDim );
	#endif

	// wykreowac wektor wynikowy
	double* pW = NULL;
	if( !CreateVector( &pW, nDim ) )
	{
		printf( "Problem: nieudana alokacja *w" );
		return 7;
	}

	// rozwiazac ukl
	LayoutEqu( pMatInv, pB, pW, nDim );

	// wyniki
	printf( "\n Wektor wynikowy ukladu rownan:" );
	PrintVector( pW, nDim );


	// zwolnic pamiec
	DeleteMatrix( &pMatrix, nDim );
	DeleteMatrix( &pMatInv, nDim );
	free( pB );
	free( pW );
	pB = NULL;
	pW = NULL;
	return 0;
}

void ReadData( FILE* fin, double** pMatrix, double* b, int nDim )
{
	for( int i = 0; i < nDim; i++, pMatrix++ )
	{
		double* p = *pMatrix;
		for( int j = 0; j < nDim; j++ ) fscanf( fin, "%lf", p++ );
		fscanf( fin, "%lf", b++ );
	}
	return;
}

int CreateVector( double** pv, int n )
{
	*pv = (double*)malloc( n * sizeof( double ) );
	if( !*pv ) return 0;
	memset( *pv, 0, n * sizeof( double ) );
	return 1;
}

void PrintVector( double* pv, int nDim )
{
	printf("\n [  " );
	for( int k = 0; k < nDim; k++ ) printf( "%.2f  ", *pv++ );
	printf( "]\n" );
	return;
}