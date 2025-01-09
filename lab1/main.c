#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void print_matrix(int n, float **matrix)
{	 // вывод матрицы
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%g ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int simetry(int n, float **matrix)
{	 // проверяем симметричность матрицы
	 //для того чтобы знать есть ли комплексные значения
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (matrix[i][j] != matrix[j][i])
			{
				return 0;
			}
		}
	}
	return 1;
}
float *opredelitel(float x1, float x2, float x3, float x4)
{
	// получаем уравнение из блока 2*2 для опеределителя если блок |x1-A x2|
	// значит получим уравненние A^2 -(x1+x4)*A+(x1*x4-x3*x2)       |x3 x4-A|
	float discriminant = (x1 + x4) * (x1 + x4) - 4 * (x1 * x4 - x3 * x2);
	float *ans = (float *)malloc(3 * sizeof(float *));

	if (discriminant < 0)
	{
		ans[0] = 1;	   // записываем что у нас в ответе будет присутствовать i;
		ans[1] = -((x1 + x4) / 2);				   // - коэф B
		ans[2] = sqrt(-1 * (discriminant)) / 2;	   // коэф который будет перед i
	}
	else
	{
		ans[0] = 0;	   // записываем что у нас в ответе нет i;
		ans[1] = -((x1 + x4) / 2) + sqrt((discriminant)) / 2;
		ans[2] = -((x1 + x4) / 2) - sqrt((discriminant)) / 2;
	}
	return ans;
}

float scalar_mult(float *vec1, float *vec2, int n)
{	 // умножение на скаляр
	float res = 0;
	for (int i = 0; i < n; i++)
	{
		res += vec1[i] * vec2[i];
	}
	return res;
}

float *prj_ba(float *a, float *b, int n)
{	 // проекция b на a
	float *res = (float *)malloc(n * sizeof(float));
	float scal = scalar_mult(a, b, n) / scalar_mult(b, b, n);
	for (int i = 0; i < n; i++)
	{
		res[i] = scal * b[i];
	}
	return res;
}

float **matrix_mult_const(float *matrix[], int n, float num)
{
	float **ans = (float **)malloc(n * sizeof(float *));
	for (int i = 0; i < n; i++)
	{
		ans[i] = (float *)malloc(n * sizeof(float));
		for (int j = 0; j < n; j++)
		{
			ans[i][j] = matrix[i][j] * num;
		}
	}
	return ans;
}

float **matrix_multiplication(float *a[], float *b[], int n, float *ans[])
{	 // умножение матриц
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ans[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				ans[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return ans;
}

float **matrix_transpose(float *a[], int n, float *ans[])
{	 //  транспонирование
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ans[i][j] = a[j][i];
		}
	}
	return ans;
}

void vec_subtr(float *a, float *b, int n)
{	 // векторное умножение
	for (int i = 0; i < n; i++)
	{
		a[i] = a[i] - b[i];
	}
}

float **gram_shmidt(float *row_matrix[], int n, float **ans)
{	 // процесс Грамма-Шмидта
	// но ответ будет транспонирован , это удобно тк мы исходную матрицу умножаем на транспонированную
	//ортогональную и только потом просто на ортогональную.+ так мы можем работать с строками матрицы а не со столбцами
	float **matrix = (float **)malloc(n * sizeof(float *));
	for (int i = 0; i < n; i++)
	{
		matrix[i] = (float *)malloc(n * sizeof(float));
	}
	matrix_transpose(row_matrix, n, matrix);

	for (int i = 0; i < n; i++)
	{
		ans[0][i] = matrix[0][i];
	}
	for (int i = 1; i < n; i++)
	{
		for (int a = 0; a < n; a++)
		{
			ans[i][a] = matrix[i][a];
		}
		for (int j = 0; j < i; j++)
		{
			vec_subtr(ans[i], prj_ba(matrix[i], ans[j], n), n);
		}
	}
	return ans;
}

float **normirovanie(float *matrix[], int n, float **res)
{	 // нормируем маттрицу Грамма-Шмидта
	res = gram_shmidt(matrix, n, res);
	float *coef = (float *)malloc(n * sizeof(float));
	for (int i = 0; i < n; i++)
	{
		float norma = 0;
		for (int j = 0; j < n; j++)
		{
			norma += res[i][j] * res[i][j];
		}
		coef[i] = sqrt(norma);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			res[i][j] = res[i][j] / coef[i];	//делим на длину вектора
		}
	}
	free(coef);
	return res;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Слишком мало аргументов.\n");
	}
	if (argc > 3)
	{
		printf("Слишком много аргументов.\n");
	}
	float **matrix;
	int dimension;

	char *filename = argv[1];
	char *out_filename = argv[2];
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Проблема открытия файла.\n");
		return 1;
	}

	fscanf(file, "%d", &dimension);

	// выделяем память для матрицы
	matrix = (float **)malloc(dimension * sizeof(float *));
	if (matrix == NULL)
	{
		printf("Проблема выделения памяти.\n");
		return 1;
	}

	for (int i = 0; i < dimension; i++)
	{
		matrix[i] = (float *)malloc(dimension * sizeof(float));
		if (matrix[i] == NULL)
		{
			printf("Проблема выделения памяти для колонки %d.\n", i);
			return 1;
		}
		for (int j = 0; j < dimension; j++)
		{
			fscanf(file, "%f", &matrix[i][j]);
		}
	}
	fclose(file);

	int n = dimension;
	int is_simetry = simetry(n, matrix);	//проверяем симметричность исходной матрицы пока ее не преобразовали
	float **Q = (float **)malloc(n * sizeof(float *));
	for (int i = 0; i < n; i++)
	{
		Q[i] = (float *)malloc(n * sizeof(float));
	}

	float **promejutok = (float **)malloc(n * sizeof(float *));
	for (int i = 0; i < n; i++)
	{
		promejutok[i] = (float *)malloc(n * sizeof(float));
	}

	float **A = (float **)malloc(n * sizeof(float *));
	for (int i = 0; i < n; i++)
	{
		A[i] = (float *)malloc(n * sizeof(float));
	}

	for (int i = 0; i < 1000; i++)
	{
		normirovanie(matrix, n, A);	   // нормируем матрицу в A притоми она уже транспонирована тк мы
		//в функции Грамма-Шмидта используем строки
		matrix_transpose(A, n, Q);	  // транспонируем ее для получения 'нормальной' ортогональной матрицы
		matrix_multiplication(A, matrix, n, promejutok);
		matrix_multiplication(promejutok, Q, n, matrix);
	}

	FILE *fp = fopen(out_filename, "w");

	// записываем ответ
	if (is_simetry)
	{	 // проверяем будут ли комплексные значения
		for (int i = 0; i < n; i++)
		{
			fprintf(fp, "%g\n", matrix[i][i]);
		}
	}
	else
	{
		for (int i = 0; i < n - 1; i += 2)
		{

			float *roots = opredelitel(matrix[i][i], matrix[i + 1][i], matrix[i][i + 1], matrix[i + 1][i + 1]);	   //выделили
																												   //блок
			if (roots[0] != 0)
			{

				fprintf(fp, "%g ", 0 + roots[1]);
				fprintf(fp, "%c", '+');
				fprintf(fp, "%g", roots[2]);
				// дописываем i
				fprintf(fp, "%c\n", 'i');
				// тоже но со знаком -

				fprintf(fp, "%g ", 0 + roots[1]);

				fprintf(fp, "%c", '-');
				fprintf(fp, "%g", roots[2]);
				// дописываем i
				fprintf(fp, "%c\n", 'i');
			}
			else
			{
				fprintf(fp, "%g\n", roots[1]);
				fprintf(fp, "%g\n", roots[2]);
			}
			free(roots);
		}
		if (n % 2 != 0)
		{
			fprintf(fp, "%g\n", matrix[n - 1][n - 1]);	  // так как только эту клетку не учли в цикле с блоками
		}
	}

	fclose(file);
	//очищаем память
	for (int i = 0; i < dimension; i++)
	{
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}
