#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ZERO_THRESH 0.0000001

int rows = 4;
int columns = 4;

int pivotRow = 0;
int pivotColumn = 0;

int augBarLocation = -1;

double prevMatrix[24][24];
double matrix[24][24];

// Prototypes
void mtrxSwap(double matrix[24][24], int row1, int row2);
void mtrxAdd(double matrix[24][24], int targetRow, int sourceRow, double factor);
void mtrxMult(double matrix[24][24], int row, double factor);
void cpyMatrix(double fromMatrix[24][24], double toMatrix[24][24]);
void mtrxFromFile(const char filename[]);

bool pickAndSwapPivot();
void printMatrix();

int main(int argc, char const *argv[])
{
	printf("\nEnter matrix filename, or type 'enter' to enter.\n");
	char promptString[100];
	scanf("%s", promptString);

	if (strcmp(promptString, "enter") == 0)
	{
		printf("Enter the number of rows: ");
		scanf("%d", &rows);

		printf("Enter the number of columns: ");
		scanf("%d", &columns);

		printf("\nNote: you can enter '|' to place the augmentation bar.\n");

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				double input;
				printf("\nRow %d Column %d: ", i+1, j+1);
				scanf("%lf", &(input));

				matrix[i][j] = input;
			}
		}
	}
	else {
		mtrxFromFile(promptString);
	}

	// Backup
	cpyMatrix(matrix, prevMatrix);

	printf("\n");

	// Main loop
	while (true) {

		// Print matrix
		printMatrix();

		// Get command
		printf("\nCommand: ('a'dd, 'm'ul, 's'wap, 'u'ndo, 'r'un solver): ");
		char command[5];
		scanf("%s", command);

		if (command[0] == 'a')
		{
			// Backup
			cpyMatrix(matrix, prevMatrix);

			int row;
			printf(" target row: ");
			scanf("%d", &row);

			int srcRow;
			printf(" source row: ");
			scanf("%d", &srcRow);

			double factor;
			printf(" factor: ");
			scanf("%lf", &factor);

			mtrxAdd(matrix, row, srcRow, factor);
		}
		else if (command[0] == 'm')
		{
			// Backup
			cpyMatrix(matrix, prevMatrix);

			int row;
			printf(" row: ");
			scanf("%d", &row);

			double factor;
			printf(" factor: ");
			scanf("%lf", &factor);

			mtrxMult(matrix, row, factor);
		}
		else if (command[0] == 's')
		{
			// Backup
			cpyMatrix(matrix, prevMatrix);

			int row1;
			printf(" target row: ");
			scanf("%d", &row1);

			int row2;
			printf(" source row: ");
			scanf("%d", &row2);

			mtrxSwap(matrix, row1, row2);
		}
		else if (command[0] == 'u')
		{
			printf("\n\nUNDO. You may only undo once at a time.\n");
			cpyMatrix(prevMatrix, matrix);
		}
		else if (command[0] == 'r')
		{
			// Backup
			cpyMatrix(matrix, prevMatrix);

			// Rest all pivots
			pivotRow = 0;
			pivotColumn = 0;

			while (pickAndSwapPivot()) {

				if (matrix[pivotRow - 1][pivotColumn - 1] != 1)
				{
					// Scale
					double factor = 1.0 / matrix[pivotRow - 1][pivotColumn - 1];

					//printf("mtrxMult(matrix, %d, %lf)\n", pivotRow, factor);
					mtrxMult(matrix, pivotRow, factor);
				}

				// Clear column
				for (int i = 0; i < rows; ++i)
				{
					if (i == pivotRow - 1) continue;

					double value = matrix[i][pivotColumn - 1];
					if (abs(value) >= ZERO_THRESH)
					{
						// Subtract multiples
						mtrxAdd(matrix, i + 1, pivotRow, -value);
					}
				}

				printMatrix();
			}

			printf("\nSolved matrix.\n");
		}
		else {
			printf("\n\nRETRY. Use only one letter.\n");
		}

		printf("\n");
	}

	return 0;
}

void cpyMatrix(double fromMatrix[24][24], double toMatrix[24][24]) {

	// Make copy of matrix for undo
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			toMatrix[i][j] = fromMatrix[i][j];
		}
	}
}

void mtrxSwap(double matrix[24][24], int row1, int row2) {

	row1--;
	row2--;

	for (int i = 0; i < columns; ++i)
	{
		double temp = matrix[row1][i];
		matrix[row1][i] = matrix[row2][i];
		matrix[row2][i] = temp;
	}
}
void mtrxAdd(double matrix[24][24], int targetRow, int sourceRow, double factor) {

	sourceRow--;
	targetRow--;

	for (int i = 0; i < columns; ++i)
	{
		matrix[targetRow][i] += matrix[sourceRow][i] * factor;
	}
}
void mtrxMult(double matrix[24][24], int row, double factor) {

	row--;

	for (int i = 0; i < columns; ++i)
	{
		matrix[row][i] *= factor;
	}
}

void mtrxFromFile(const char filename[]) {

	int column = 0;
	int row = 0;
	columns = 1;
	rows = 1;

	char line[100];
	FILE* matrixFile = fopen(filename, "r");

	if (matrixFile)
	{
		while (fgets (line, sizeof(line), matrixFile)) {

			char* token = strtok(line, " ");
			while (token) {

				if (token != NULL)
				{
					if (token[0] == '|') {
						augBarLocation = column;
					}
					else {
						double value;
						if (sscanf(token, "%lf", &value) != 0) {
							matrix[row][column] = value;
							column++;
							if (column >= columns) columns = column;
						}
					}
				}
				token = strtok(NULL, " ");
			}

			row++;
			column = 0;
			if (row >= rows) rows = row;
		}
		if (ferror(matrixFile))
		{
			fprintf(stderr,"Could not read file.\n");
			abort();
		}
		fclose(matrixFile);
	}
	else {
		printf("\nInvalid file.\n\n");
		exit(-1);
	}
}

// Returns whether a pivot was able to be picked
// Does
bool pickAndSwapPivot() {

	int potentialPivotRow = pivotRow + 1;
	int potentialPivotColumn = pivotColumn + 1;

	// If matrix is 0 then don't use that point as a pivot
	while (abs(matrix[potentialPivotRow - 1][potentialPivotColumn - 1]) < ZERO_THRESH ) {
		potentialPivotRow++;

		if (potentialPivotRow > rows)
		{
			potentialPivotRow = pivotRow + 1;
			potentialPivotColumn++;

			if (potentialPivotColumn > columns) return false;
		}
	}

	if (potentialPivotRow != pivotRow + 1) mtrxSwap(matrix, potentialPivotRow, pivotRow + 1);
	pivotRow++;
	pivotColumn = potentialPivotColumn;

	//printf("Pivot picked at row %d column %d: %lf\n", pivotRow, pivotColumn, matrix[pivotRow - 1][pivotColumn - 1]);

	return true;
}

void printMatrix() {
	printf("\nMatrix:\n");
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (j == augBarLocation) printf("| ");

			double value = matrix[i][j];
			if (value >= ZERO_THRESH) printf("%.3lf ", value);
			else printf("%.2lf ", value);
		}
		printf("\n");
	}
}



