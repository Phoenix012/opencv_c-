#pragma once

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn.hpp"

using namespace cv;
using namespace std;

class QuickDemo
{
private:
	int test = 1;


public:
	void colorSpace_Demo(Mat& image);
	void mat_create_Demo();
	void pixel_visit_Demo(Mat& image);
	void operator_demo(Mat& image);
	void tracking_bar_demo(Mat& image);
	void tracking_bar_demo_parameter(Mat& image);
	void tracking_bar_demo_2bar(Mat& image);
	void key_demo(Mat& image);
	void color_stype_demo(Mat& image);
	void bitwise_demo(Mat& image);
	void channel_demo(Mat& image);
	void inRange_demo(Mat& image);
	void pixel_statistic_demo(Mat& image);
	void drawing_demo(Mat& image);
	void random_drawing();
	void polyline_drawing_demo();
	void mouse_drawing_demo(Mat& image);
	void norm_demo(Mat& image);
	void resize_demo(Mat& image);
	void flip_demo(Mat& image);
	void rotate_demo(Mat& image);
	void video_demo();
	void histogram_demo(Mat& image);
	void histogram_2d_demo(Mat& image);
	void histogram_eq_demo(Mat& image);
	void blur_demo(Mat& image);
	void gaussian_blur_demo(Mat& image);
	void bifilter_demo(Mat& image);
	void face_detection_demo();
};
