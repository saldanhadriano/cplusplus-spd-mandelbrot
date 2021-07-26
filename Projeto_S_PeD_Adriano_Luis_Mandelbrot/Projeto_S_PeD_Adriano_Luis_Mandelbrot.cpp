#include <fstream>
#include <iostream>>
#include <omp.h>

using namespace std;

int findMandelbrot(double cr, double ci, int max_iterations)
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < max_iterations && zr * zr + zi * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}

	return i;
}

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;
}

int main()
{

	// Pega o arquivo de entrada
	ifstream fin("input.txt");
	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;

	if (!fin)
	{
		cout << "Could not open file!" << endl;
		return 1;
	}

	fin >> imageWidth >> imageHeight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin.close(); //Boa prática, não obrigatorio

				 // Abre o arquivo de saída, escreve o cabeçalho do arquivo PPM...
	ofstream fout("output_image.ppm");
	fout << "P3" << endl; // "Magic Number" - PPM file //"Número Mágico" - Arquivo PPM
	fout << imageWidth << " " << imageHeight << endl; //Dimensões
	fout << "255" << endl; // Valor máximo de um pixel, valor R,G,B

	double start = omp_get_wtime();

	//PARALELIZAR
	// Para cada pixel
	for (int y = 0; y < imageHeight; y++) // Linhas...
	{
		for (int x = 0; x < imageWidth; x++) // Pixels na linha (colunas)...
		{
			// ... Procura o valor real e imaginario para c, correspondente a
			//     x, y pixel na imagem.
			double cr = mapToReal(x, imageWidth, minR, maxR);
			double ci = mapToImaginary(y, imageHeight, minI, maxI);

			// ... Procura o numero de iteracoes na formula de Mandelbrot
			//     usando c.
			int n = findMandelbrot(cr, ci, maxN);

			// ... Map the resulting number to an RGP value
			// ... Mapeia número resultante para um valor RGB
			int r = (n % 256);
			int g = (n % 256);
			int b = (n % 256);

			// ... Output it to an image
			// << shift-left deslocamento de bits
			fout << r << " " << g << " " << b << " ";
		}
		fout << endl;
	}
	double end = omp_get_wtime();
	fout.close();

	cout << "Finished!" << endl;

	printf_s("Tempo Decorrido = %.16g minutos\n", ((end - start) / 60));
	//("Inicio = %.16g minutos\nFim = %.16g minutos\nd, start, end")
	system("pause");
	return 0;
}