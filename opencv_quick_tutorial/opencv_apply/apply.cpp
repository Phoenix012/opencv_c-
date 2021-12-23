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

	imwrite("C:/images/hsv.tif", dst);
}

void OpencvHelper::mask_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	int cols = image.cols*image.channels();
	int rows = image.rows;
	int offset = image.channels();
	for (int row = 1;row < (rows - 1);row++)
	{
		uchar* previousRow = image.ptr<uchar>(row-1);
		uchar* currentRow = image.ptr<uchar>(row);
		uchar* nextRow = image.ptr<uchar>(row+1);
		uchar* output = dst.ptr<uchar>(row);
		for (int col = offset; col < cols; col++)
		{
			output[col] = saturate_cast<uchar>(5 * currentRow[col] - (currentRow[col - offset] + currentRow[col + offset] + previousRow[col] + nextRow[col]));
		}
	}
	imshow("mask demo", dst);


	Mat src2 = image.clone();
	Mat dst2 = Mat::zeros(image.size(), image.type());
	//0 -1  0
	//-1 5 -1
	//0 -1  0
	Mat kernal = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	filter2D(src2, dst2, src2.depth(), kernal);
	imshow("filter2D mask", dst2);
}

void OpencvHelper::blend_demo()
{
	Mat src1 = imread("C:/images/hist_01.jpg");
	Mat src2 = imread("C:/images/hist_02.jpg");
	Mat dst;
	if (src1.size() != src2.size() || src1.type() != src2.type())
	{
		cout << "could not apply addWeight";
	}
	else
	{
		double alpha = 0.5;
		addWeighted(src1, alpha, src2, 1 - alpha, 0, dst);
		imshow("add weight demo", dst);
	}
}