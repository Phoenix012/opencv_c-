#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class OpencvHelper
{
public:
	int add(int x, int y);
	void load_image_demo(Mat& image);
	void mask_demo(Mat& image);	//��߶Աȶȣ�������
	void blend_demo();			//ͼ����
	void light_contrast_demo(Mat& image);	//����&�Աȶ�
};
