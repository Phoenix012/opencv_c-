#include "apply.h"

int main(int argc, void** argv)
{
	const char* image = "C:/images/zhifang_ball.png";
	OpencvHelper oh;
	Mat src;
	src = imread(image);
	imshow("src", src);
	if (src.empty())
	{
		cout << "could not load image.." << endl;
		return -1;
	}
	oh.load_image_demo(src);

	waitKey(0);
	destroyAllWindows();

	return 0;
}