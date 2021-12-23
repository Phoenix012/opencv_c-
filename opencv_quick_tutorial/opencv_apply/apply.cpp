#include "apply.h"

int OpencvHelper::add(int x, int y)
{
	return x + y;
}

void OpencvHelper::load_image_demo(Mat& image)
{
	Mat dst;
	cvtColor(image, dst, COLOR_BGR2HSV);
	imshow("HSV", dst);
}