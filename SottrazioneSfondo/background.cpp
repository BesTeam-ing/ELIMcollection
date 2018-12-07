/*
 * Sottrazione dello sfondo da Video o Foto
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat mediaBack(Mat imgs[], int size)
{
	Mat media;
	imgs[0].copyTo(media);
	for (int k = 0; k < size; k++)
	{
		for(int i = 0; i < media.rows; i++)
		{
			for(int j = 0; j < media.cols; j++)
			{
				media.at<uchar>(i,j) = (imgs[k].at<uchar>(i,j) + media.at<uchar>(i,j) ) /2;
			}
		}
	}
	namedWindow("BGSub",WINDOW_NORMAL);
	imshow("BGSub",media);
	waitKey(0);
return media;
}

void movedObject(Mat media,Mat newframe)
{
	Mat temp;
	media.copyTo(temp);
	for (int i = 0; i < newframe.rows; i++)
	{
		for (int j = 0; j < newframe.cols; j++)
		{
			temp.at<uchar>(i,j) = abs(newframe.at<uchar>(i,j) - media.at<uchar>(i,j) );
		}
	}
	namedWindow("Moved",WINDOW_NORMAL);
	imshow("Moved",temp);
	namedWindow("New",WINDOW_NORMAL);
	imshow("New",newframe);
	waitKey(0);

	destroyAllWindows();
}

void analizzaImmagine(int argc, char** argv)
{
	int size = argc-2;
	Mat imgs[size], newframe;
	
	for(int i = 0; i < size; i++)
		imgs[i] = imread(argv[i+2],0);
	
	newframe = imread(argv[1],0);
	Mat media = mediaBack(imgs,size);
	movedObject(media,newframe);
	exit(0);
}

void analizzaVideo(String argv)
{
	Mat sfondo,frame;
	VideoCapture video;
	
	video = VideoCapture(argv);

	if (!video.isOpened())
		exit(1);

	namedWindow("Video",1);
	int i = 0;

	for(;;)
	{
		video >> frame;
		cvtColor(frame,frame,COLOR_BGR2GRAY);
		imshow("Video",frame);

		if (!sfondo.data)
			frame.copyTo(sfondo);
		else
		{
			for(int x = 0; x < sfondo.rows; x++)
			{
				for(int y = 0; y < sfondo.cols; y++)
				{
					sfondo.at<uchar>(x,y) = abs(frame.at<uchar>(x,y) - sfondo.at<uchar>(x,y));
					imshow("Movimento",sfondo);
					frame.copyTo(sfondo);
				}
			}
		}

		//Aspetto 30 secondi
		if (waitKey(30)==27)
		{
			cout<<"uscita!"<<endl;
			exit(0);
		}
	}
}

int main(int argc,char** argv){

	if(argc != 3)
	{
		//ssssssss
		printf("Sintassi non corretta! ./houghrette [imgPath] [Treshold]");
		return -1;
	}

	if (argc == 2)
		analizzaVideo(argv[1]);
	else 
		analizzaImmagine(argc,argv);

return 0;
}
