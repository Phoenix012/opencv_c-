#include <opencv2/opencv.hpp>
#include "quickopencv.h"
using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	/*string windowName = "input";
	Mat src = imread("C:\\4603.bmp", IMREAD_UNCHANGED);
	if (src.empty())
	{
		printf("could not load image...");
		return -1;
	}
	int depth = src.depth();
	namedWindow(windowName, WINDOW_FREERATIO);
	imshow(windowName, src);
	waitKey(0);*/

	Mat src = imread("C:\\images\\test.png");
	if (src.empty())
	{
		printf("could not load image...");
		return -1;
	}
	//cout << src << endl;
	imshow("src", src);
	
	QuickDemo qd;
	qd.gaussian_blur_demo(src);

	waitKey(0);

	destroyAllWindows();

	return 0;
}