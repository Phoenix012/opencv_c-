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
		uchar* previousRow = image.ptr<uchar>(row - 1);
		uchar* currentRow = image.ptr<uchar>(row);
		uchar* nextRow = image.ptr<uchar>(row + 1);
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
		//dst = src1*alpha + src2*beta + gamma;
		addWeighted(src1, alpha, src2, 1 - alpha, 0, dst);
		imshow("add weight demo", dst);
	}
}

void OpencvHelper::light_contrast_demo(Mat& image)
{
	int h = image.rows;
	int w = image.cols;
	Mat dst = Mat::zeros(image.size(), image.type());

	//y=alpha*x+gemma
	double alpha = 0.8;	//对比度
	double beta = 10;	//亮度
	int channel = image.channels();

	for (int row = 0;row < h;row++)
	{
		for (int col = 0; col < w; col++)
		{
			if (channel == 1)
			{
				uchar value = image.at<uchar>(row, col);
				dst.at<uchar>(row, col) = saturate_cast<uchar>(value * alpha + beta);
			}
			else if (channel == 3)
			{
				float b = image.at<Vec3b>(row, col)[0];
				float g = image.at<Vec3b>(row, col)[1];
				float r = image.at<Vec3b>(row, col)[2];

				dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(alpha * b + beta);
				dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(alpha * g + beta);
				dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(alpha * r + beta);
			}
			else
			{
				return;
			}
		}
	}
	imshow("light and contrast demo", dst);
}

void OpencvHelper::draw_demo(Mat& image)
{
	int w = image.cols;
	int h = image.rows;
	Mat dst = image.clone();

	line(dst, Point(0, 0), Point(w, h), Scalar(0, 0, 255), 2, LINE_AA);

	rectangle(dst, Rect(w / 4, h / 4, w / 2, h / 2), Scalar(0, 0, 100), 2, LINE_AA);

	circle(dst, Point(w / 2, h / 2), w / 4, Scalar(0, 100, 100), 2, LINE_AA);

	ellipse(dst, Point(w / 2, h / 2), Size(w / 2, w / 4), 45, 0, 180, Scalar(200, 0, 0), 2, LINE_AA);

	Point pts[1][5];
	pts[0][0] = Point(100, 100);
	pts[0][1] = Point(100, 200);
	pts[0][2] = Point(200, 300);
	pts[0][3] = Point(200, 100);
	pts[0][4] = Point(100, 80);
	const Point* ppts[] = { pts[0] };
	int npt[] = { 5 };

	fillPoly(dst, ppts, npt, 1, Scalar(255, 12, 200), LINE_8);


	putText(dst, String("Hello OpenCV"), Point(100, 100), FONT_HERSHEY_COMPLEX, 1.2, Scalar(100, 100, 255));

	RNG rng(12345);
	Point p1, p2;
	while (true)
	{
		p1.x = rng.uniform(0, w);
		p1.y = rng.uniform(0, h);
		p2.x = rng.uniform(0, w);
		p2.y = rng.uniform(0, h);
		Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		char key = waitKey(100);
		if (key == 27)
		{
			break;
		}
		line(dst, p1, p2, color, 2, LINE_AA);
		imshow("draw demo", dst);
	}
}