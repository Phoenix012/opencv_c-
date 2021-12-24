#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class OpencvHelper
{
public:
	int add(int x, int y);
	void load_image_demo(Mat& image);
	void mask_demo(Mat& image);	//提高对比度，更立体
	void blend_demo();			//图像混合
	void light_contrast_demo(Mat& image);	//亮度&对比度
};
