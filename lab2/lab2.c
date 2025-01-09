#include <stdio.h>
#include <zlib.h>
// Определяем типы данных и коды цветов
enum Chans
{
	GRAY = 0,
	RGB = 2,
	PLTE = 3,
	RGBA = 6
};

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: png_decoder has problems with args\n");
		return -1;
	}

	// Открываем файл для декодирования
	FILE *png_file = fopen(argv[1], "rb");
	if (png_file == NULL)
	{
		printf("Could not open file %s\n", argv[1]);
		return -1;
	}

	// Определяем тип цвета и размеры изображения
	int chans, width, height;
	fread(&chans, sizeof(int), 1, png_file);
	fread(&width, sizeof(int), 1, png_file);
	fread(&height, sizeof(int), 1, png_file);

	// В зависимости от типа цвета, читаем данные изображения и записываем их в файл PNM
	switch (chans)
	{
	case GRAY: {
		// Для серых изображений
		FILE *new_file = fopen(argv[2], "wb");
		fprintf(new_file, "P5\n%d %d\n255\n", width, height);

		// Читаем байты до конца файла
		int c;
		while ((c = fgetc(png_file)) != EOF)
			fputc(c, new_file);

		fclose(new_file);
		break;
	}
	case RGB: {
		// Для цветных изображений
		FILE *new_file = fopen(argv[2], "wb");
		fprintf(new_file, "P6\n%d %d\n255\n", width, height);

		// Читаем байты в цикле до достижения конца файла
		int c;
		while ((c = fgetc(png_file)) != EOF)
			fputc(c, new_file);

		fclose(new_file);
		break;
	}
	case PLTE: {
		// Для изображений с палитрой
		FILE *new_file = fopen(argv[2], "wb");
		fprintf(new_file, "P6\n%d %d\n255\n", width, height);

		// Читаем байты из таблицы палитры
		int table_size = 0;
		fread(&table_size, sizeof(int), 1, png_file);
		char *palette = (char *)malloc(table_size);
		int c;
		for (int i = 0; i < table_size; i++)
		{
			c = fgetc(png_file);
			palette[i] = c;
		}

		// Читаем индексы палитры до конца файла
		while ((c = fgetc(png_file)) != EOF)
		{
			// Для каждого индекса находим соответствующее значение из таблицы палитры
			fputc(palette[c], new_file);
		}

		fclose(new_file);
		free(palette);
		break;
	}
	case RGBA: {
		printf("Not support color type\n");
		return -1;
	}
	default: {
		printf("Unknown color type\n");
		return -1;
	}

		fclose(png_file);
		return 0;
	}
}
