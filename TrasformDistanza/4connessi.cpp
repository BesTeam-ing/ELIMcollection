/*
 * Trasformata della Distanza o Trasformata della Distanza Pesata mediante due scansioni
*/
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char** argv)
{
	int tresh = atoi(argv[2]); //250
	if(argc != 2 || tresh < 0 || tresh > 255)
	{
		printf("Sintassi non corretta! ./4connessi [imgPath] [0-255]");
		return -1;
	}

	Mat img = imread(argv[1],0);	
	Mat img2 = img.clone();
//Trasformo l'immagine in B/N in base al Treshold scelto
	for (int i = 0; i < img.rows; i++)
		for(int j = 0; j < img.cols; j++)
				if (img.at<uchar>(i,j) > tresh)
					img2.at<uchar>(i,j)=0;
				else
					img2.at<uchar>(i,j)=255;
				
	imshow("Binary", img2);
				
	int contatore=0;

//Scansione Diretta
	for (int i = 1; i < img2.rows-1; i++)	
		for (int j = 1; j < img2.cols-1; j++)
			{	
				if(img2.at<uchar>(i,j) != 0) // cioè ho trovato un pixel diverso da 0 cioè diverso dallo sfondo
					{	
						contatore=0;
						int vettore[9];
						//int mask[9] = {3,2,3,2,0,2,3,2,3}; //Mask per la distanza Pesata
						int mask[9] = {1,1,1,1,0,1,1,1,1};
					
						
							
						vettore[3] = img2.at<uchar>(i-1,j+1);
						vettore[2] = img2.at<uchar>(i-1,j);
						vettore[1] = img2.at<uchar>(i-1,j-1);		
						vettore[0] = img2.at<uchar>(i,j-1);

						int min2 = 444; //Un valore piu alto di 255

						int indice = 0;
						for (int k = 0; k < 4; k++)
							if (vettore[k] < min2)
							{
								min2 = vettore[k];
								indice = k;
							}
						img2.at<uchar>(i,j) = min2 + mask[indice];
					}
			}

//Scansione Inversa
	for (int i = img2.rows-1; i > 0; i--)
		for (int j = img2.cols-1; j > 0; j--)
			{
				if(img2.at<uchar>(i,j) != 0)
					{
						contatore=0;
						int vettore[9];
						//int mask[9] = {3,2,3,2,0,2,3,2,3}; //Mask per la distanza Pesata
						int mask[9] = {1,1,1,1,0,1,1,1,1};
						
						vettore[4] = img2.at<uchar>(i,j) + mask[4];
						vettore[5] = img2.at<uchar>(i,j+1) + mask[5];
						vettore[6] = img2.at<uchar>(i+1,j+1) + mask[8];		
						vettore[7] = img2.at<uchar>(i+1,j) + mask[7];
						vettore[8] = img2.at<uchar>(i+1,j-1) + mask[6];

						int min = 444; //Un valore qualsiasi piu grande di 255
						
						for (int k = 4; k < 9; k++)
							if (vettore[k] < min)
							{
								min = vettore[k];
							}
							
						img2.at<uchar>(i,j) = min;
				}
			}
	imshow("SRC",img);
	imshow("Transform",img2);
	waitKey(0);

return 0;
}
