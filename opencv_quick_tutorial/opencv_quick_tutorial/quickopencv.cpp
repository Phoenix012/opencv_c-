#include "quickopencv.h"

using namespace cv;
using namespace std;

void QuickDemo::colorSpace_Demo(Mat& image)
{
	Mat gray, hsv;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	cvtColor(image, hsv, COLOR_BGR2HSV);

	imshow("gray", gray);
	imshow("hsv", hsv);

	imwrite("gray.bmp", gray);
	imwrite("hsv.bmp", hsv);
}

void QuickDemo::mat_create_Demo()
{
	//Mat image;
	//Mat m1, m2;
	//m1 = image.clone();
	//image.copyTo(m2);

	Mat m3;
	m3 = Mat::zeros(Size(400, 400), CV_8UC3);
	//cout << m3 << endl;
	m3 = Scalar(0, 0, 250);
	cout << "width: " << m3.cols << " height: " << m3.rows << " channel: " << m3.channels() << endl;
	imshow("M3", m3);


	Mat m4;
	m4 = m3.clone();
	m4 = Scalar(0, 255, 255);
	imshow("M4", m4);
}

void QuickDemo::pixel_visit_Demo(Mat& image)
{
	int width = image.cols;
	int height = image.rows;
	int dims = image.channels();
#pragma region �����±귽λͼ��Ԫ��
	for (int row = 0;row < height;row++)	//
	{
		for (int col = 0;col < width;col++)
		{
			if (dims == 1)
			{
				//�Ҷ�ͼ��
				int pixelValue = image.at<uchar>(row, col);
				image.at<uchar>(row, col) = 255 - pixelValue;	//��ת
			}
			if (dims == 3)
			{
				//��ɫͼ��
				Vec3b bgr = image.at<Vec3b>(row, col);
				image.at<Vec3b>(row, col)[0] = 255 - bgr[0];
				image.at<Vec3b>(row, col)[1] = 255 - bgr[1];
				image.at<Vec3b>(row, col)[2] = 255 - bgr[2];

			}
		}
	}
#pragma endregion

#pragma region ָ�����
	/*for (int row = 0;row < height;row ++)
	{
		uchar* currentRow = image.ptr<uchar>(row);
		for (int col = 0; col < width; col++)
		{
			if (dims == 1)
			{
				int pixelValue = *currentRow;
				*currentRow++ = 255 - pixelValue;
			}
			if (dims == 3)
			{
				*currentRow++ = 255 - *currentRow;
				*currentRow++ = 255 - *currentRow;
				*currentRow++ = 255 - *currentRow;
			}
		}
	}*/
#pragma endregion


	imshow("pixel visit", image);
}

void QuickDemo::operator_demo(Mat& image)
{
	Mat dst;
	dst = image + Scalar(50, 50, 50);
	imshow("operator image +", dst);

	dst = image - Scalar(50, 50, 50);
	imshow("operator image -", dst);

	dst = image / Scalar(5, 5, 5);
	imshow("operator image /", dst);

	multiply(image, Scalar(2, 2, 2), dst);
	imshow("operator image *", dst);

	//�Լ�д�ıȽϱ��ļӷ�
	int width = image.cols;
	int height = image.rows;
	int dims = image.channels();
	dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(50, 50, 50);
	for (int row = 0;row < height;row++)	//
	{
		for (int col = 0;col < width;col++)
		{
			//��ɫͼ��
			Vec3b P1 = image.at<Vec3b>(row, col);
			Vec3b P2 = m.at<Vec3b>(row, col);
			//saturate_cast --> 
			dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(P1[0] + P2[0]);
			dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(P1[1] + P2[1]);
			dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(P1[2] + P2[2]);
		}
	}
	imshow("�Լ�д�ıȽϱ��ļӷ�", dst);


	//Opencv ���Դ��ļӼ��˳�����
	add(image, m, dst);
	imshow("add", dst);

	m = Scalar(10, 10, 10);
	divide(image, m, dst);
	imshow("divide", dst);

	m = Scalar(5, 5, 5);
	divide(image, m, dst);
	imshow("divide", dst);
}

Mat src, dst, m;
int lightness = 50;
int maxValue = 100;

static void on_track(int, void*)
{
	m = Scalar(lightness, lightness, lightness);
	add(src, m, dst);
	imshow("track bar demo", dst);
}
void QuickDemo::tracking_bar_demo(Mat& image)
{
	dst = Mat::zeros(image.size(), image.type());
	m = Mat::zeros(image.size(), image.type());
	src = image;
	namedWindow("track bar demo", WINDOW_AUTOSIZE);
	createTrackbar("adjust value", "track bar demo", &lightness, maxValue, on_track);
	on_track(50, 0);
}

static void on_track_parameter(int b, void* userdata)
{
	Mat src = *(Mat*)(userdata);
	Mat dst = Mat::zeros(src.size(), src.type());
	Mat m = Mat::zeros(src.size(), src.type());
	m = Scalar(b, b, b);
	add(src, m, dst);
	imshow("track bar demo parameter", dst);
}

void QuickDemo::tracking_bar_demo_parameter(Mat& image)
{
	namedWindow("track bar demo parameter", WINDOW_AUTOSIZE);
	int lightness = 50;
	int maxValue = 100;
	createTrackbar("adjust value", "track bar demo parameter", &lightness, maxValue, on_track_parameter, (void*)(&image));
	on_track_parameter(50, &image);
}

string winLightContrast = "ͨ��AddWeight�������ȺͶԱȶ�";
string barLightness = "��������";
string barContrast = "���ڶԱȶ�";

static void OnLightness(int b, void* userdata)
{
	Mat src = *((Mat*)userdata);
	Mat dst = Mat::zeros(src.size(), src.type());
	Mat m = Mat::zeros(src.size(), src.type());
	//addWeighted(src, alpha, m, beta, gama, dst);
	//dst = src1*alpha + src2*beta + gamma
	//����alpha =1, beta = 0, gamma = b, ��dst = src+b, ��Ϊ��������ʹ��
	addWeighted(src, 1.0, m, 0.0, b, dst);
	imshow(winLightContrast, dst);
}
static void OnContrast(int b, void* userdata)
{
	Mat src = *((Mat*)userdata);
	Mat dst = Mat::zeros(src.size(), src.type());
	Mat m = Mat::zeros(src.size(), src.type());
	//addWeighted(src, alpha, m, beta, gama, dst);
	//dst = src1*alpha + src2*beta + gamma
	//����alpha =b/100.0, beta = 0, gamma = 0, ��dst = src * (b/100), ��Ϊ���ڶԱȶ�ʹ��
	double contrast = b / 100.0;
	addWeighted(src, contrast, m, 0.0, 0, dst);
	imshow(winLightContrast, dst);
}

void QuickDemo::tracking_bar_demo_2bar(Mat& image)
{
	namedWindow(winLightContrast, WINDOW_AUTOSIZE);
	int lightness = 50;
	int maxLightness = 100;
	int contrast = 100;
	int maxContrast = 200;
	createTrackbar(barLightness, winLightContrast, &lightness, maxLightness, OnLightness, (void*)(&image));
	createTrackbar(barContrast, winLightContrast, &contrast, maxContrast, OnContrast, (void*)(&image));
	OnLightness(50, &image);
}

void QuickDemo::key_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	while (true)
	{
		int c = waitKey(100);
		cout << c << endl;
		if (c == 27)	//ESC
		{
			cout << "You enter Key ESC" << endl;
			break;
		}
		if (c == 49)	//Key #1
		{
			cout << "You enter Key #1" << endl;
			cvtColor(image, dst, COLOR_BGR2GRAY);
		}
		if (c == 50)	//Key #2
		{
			cout << "You enter Key #2" << endl;
			cvtColor(image, dst, COLOR_BGR2HSV);
		}
		if (c == 51)
		{
			cout << "You enter Key #3" << endl;
			dst = Scalar(50, 50, 50);
			add(image, dst, dst);
		}
		imshow("Key Demo", dst);
	}
}

static const String ColorMaps[] = { "Autumn", "Bone", "Jet", "Winter", "Rainbow", "Ocean", "Summer", "Spring",
									"Cool", "HSV", "Pink", "Hot", "Parula", "Magma", "Inferno", "Plasma", "Viridis",
									"Cividis", "Twilight", "Twilight Shifted", "Turbo", "User defined (random)" };
void QuickDemo::color_stype_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	int colorType = 0;
	while (true)
	{
		int c = waitKey(500);
		if (c == 27)
			break;

		//colorType = QuickDemo::COLORMAP_WINTER;
		//applyColorMap(image, dst, colorType);

		applyColorMap(image, dst, colorType % 22);

		ColormapTypes index = static_cast<ColormapTypes>(colorType);
		cout << "Color map type: " << index % 22 << " = " << ColorMaps[index % 22] << endl;

		colorType++;
		imshow("Color type", dst);
	}
}

///λ����
void QuickDemo::bitwise_demo(Mat& image)
{
	Mat m1 = Mat::zeros(Size(256, 256), CV_8UC3);
	Mat m2 = Mat::zeros(Size(256, 256), CV_8UC3);

	rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(150, 150, 80, 80), Scalar(0, 255, 255), -1, LINE_8, 0);

	imshow("m1", m1);
	imshow("m2", m2);

	Mat dst;
	//And��
	bitwise_and(m1, m2, dst);
	imshow("bitwise_and", dst);
	//Or��
	bitwise_or(m1, m2, dst);
	imshow("bitwise_or", dst);
	//Not��
	bitwise_not(image, dst);
	imshow("bitwise_not", dst);
	dst = ~dst;
	imshow("~dst", dst);
	//Xor���
	bitwise_xor(m1, m2, dst);
	imshow("bitwise_xor", dst);
}

void QuickDemo::channel_demo(Mat& image)
{
	vector<Mat> mv;
	split(image, mv);

	imshow("��ɫ", mv[0]);
	imshow("��ɫ", mv[1]);
	imshow("��ɫ", mv[2]);

	Mat dst;
	/*mv[1] = 0;
	mv[2] = 0;
	merge(mv, dst);
	imshow("��ɫ", dst);*/

	/*mv[0] = 0;
	mv[2] = 0;
	merge(mv, dst);
	imshow("��ɫ", dst);*/

	/*mv[0] = 0;
	mv[1] = 0;
	merge(mv, dst);
	imshow("��ɫ", dst);*/

	dst = Mat::zeros(image.size(), image.type());
	int from_to[] = { 0,2,1,1,2,0 };	//��0ͨ������2ͨ������1ͨ���ڵ�1ͨ������2ͨ������0ͨ��
	mixChannels(&image, 1, &dst, 1, from_to, 3);
	//				����from ����destination  3��ͨ��

	imshow("ͨ�����", dst);
}

//////////////////////////////////////////////////////////////////////////
//����ɫ�����µ��˿ٳ���:greenback.png
//Ȼ������ŵ���ɫ������
void QuickDemo::inRange_demo(Mat& image)
{
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);//ת��HSV��ɫͼ��
	Mat mask;
	inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask);
	imshow("mask", mask);// ���񱻿ٳ���������ɫΪ255�� ����Ϊ0

	Mat redback = Mat::zeros(image.size(), image.type());
	redback = Scalar(40, 40, 200);
	bitwise_not(mask, mask);	//maskλȡ����������Ϊ255�� ����Ϊ0
	imshow("new mask", mask);

	//copies those GpuMat elements to "m" that are marked with non-zero mask elements (Non-Blocking call)
	image.copyTo(redback, mask);	//��image������redback�� ֻ����maks�б����Ϊ�����Ԫ�أ�Ϊ0������λ�þͲ�����
	imshow("roi������ȡ", redback);
}

void QuickDemo::pixel_statistic_demo(Mat& image)
{
	double minv, maxv;
	Point minP, maxP;

	vector<Mat> mv;
	split(image, mv);
	for (int i = 0;i < mv.size();i++)
	{
		//����ÿ��ͨ�������/Сֵ
		minMaxLoc(mv[i], &minv, &maxv, &minP, &maxP, Mat());	//ֻ���㵥��ͨ���������Сֵ
		cout << "channel " << i << " ��Сֵ: " << minv << " ���ֵ: " << maxv << endl;
	}

	//Mean �� Dev, ��һ��3*1 �ľ���
	// mean:	[40;
	//			40;
	//			200]
	//red back image's stddev: [0;
	//							0;
	//							0]
	//
	//
	Mat mean, stddev;
	meanStdDev(image, mean, stddev);
	for (int j = 0;j < mean.rows;j++)
	{
		cout << "channel " << j << " mean: " << mean.at<double>(j, 0) << endl;
		cout << "channel " << j << " stddev: " << stddev.at<double>(j, 0) << endl;
	}

	Mat redback = Mat::zeros(image.size(), image.type());
	redback = Scalar(40, 40, 200);
	imshow("redback", redback);
	meanStdDev(redback, mean, stddev);
	cout << "red back image's mean: " << mean << endl;
	//��ɫ��ͼƬ�����ر�С������ֵ���Ա���ͼ�����Ч��Ϣ���Աȶ�
	//����С��˵��ͼƬЯ����Ч��ϢС���Աȶȵ�
	cout << "red back image's stddev: " << stddev << endl;
}

void QuickDemo::drawing_demo(Mat& image)
{
	Mat bg = Mat::zeros(image.size(), image.type());
	rectangle(bg, Rect(100, 100, 250, 300), Scalar(0, 0, 255), -1, LINE_8, 0);
	circle(bg, Point(350, 400), 15, Scalar(255, 0, 0), -1, LINE_4, 0);
	line(bg, Point(100, 100), Point(350, 400), Scalar(0, 255, 0), 4, LINE_AA, 0);

	RotatedRect rrt;
	rrt.center = Point(200, 200);
	rrt.size = Size(100, 200);
	rrt.angle = 30;
	ellipse(bg, rrt, Scalar(0, 255, 255), 2, LINE_8);
	imshow("bg", bg);

	Mat dst;
	addWeighted(image, 0.7, bg, 0.3, 0, dst);
	imshow("������ʾ", dst);
}

void QuickDemo::random_drawing()
{
	Mat canvas = Mat::zeros(Size(500, 500), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	RNG rng(12345);
	while (true)
	{
		char key = waitKey(100);
		if (key == 27)
		{
			break;
		}
		//�������
		int X1 = rng.uniform(0, w);	//����P1.X
		int Y1 = rng.uniform(0, h);	//P1.Y
		int X2 = rng.uniform(0, w);	//P2.X
		int Y2 = rng.uniform(0, h);	//P2.Y
		//�����ɫ
		int b = rng.uniform(0, 255);
		int g = rng.uniform(0, 255);
		int r = rng.uniform(0, 255);
		//canvas = Scalar(0, 0, 0);	//ÿ�ζ���������0�� ����ȥ��ֻ��һ�����ڻ�
		line(canvas, Point(X1, Y1), Point(X2, Y2), Scalar(b, g, r), 1, LINE_AA);
		imshow("random drawing", canvas);
	}
}

void QuickDemo::polyline_drawing_demo()
{
	Mat canvas = Mat::zeros(Size(1000, 1000), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	Point p1(10, 10);
	Point p2(30, 15);
	Point p3(30, 35);
	Point p4(25, 45);
	Point p5(5, 45);

	//��ployLines���ƣ���fillPoly���
	vector<Point> pts;	//����һ��Point���͵ļ���
	pts.push_back(p1);
	pts.push_back(p2);
	pts.push_back(p3);
	pts.push_back(p4);
	pts.push_back(p5);
	polylines(canvas, pts, true, Scalar(255, 0, 0), 2, LINE_AA, 0);	//����
	fillPoly(canvas, pts, Scalar(100, 0, 0), LINE_8, 0);			//���

	//��drawContous����+���
	vector<Point> pts1;
	Point p11(100, 100);
	Point p22(300, 150);
	Point p33(300, 350);
	Point p44(250, 450);
	Point p55(50, 450);
	pts1.push_back(p11);
	pts1.push_back(p22);
	pts1.push_back(p33);
	pts1.push_back(p44);
	pts1.push_back(p55);

	vector<Point> pts2;
	Point p111(200, 200);
	Point p222(600, 350);
	Point p333(600, 550);
	Point p444(350, 650);
	Point p555(220, 750);
	pts2.push_back(p111);
	pts2.push_back(p222);
	pts2.push_back(p333);
	pts2.push_back(p444);
	pts2.push_back(p555);

	vector<vector<Point>> contours;	//����һ������μ��ϣ�����Ԫ�ض����ɵ㼯����ɵĶ����
	contours.push_back(pts1);
	contours.push_back(pts2);

	//contourIdx Parameter indicating a contour to draw.If it is negative, all the contours are drawn.
	//contourIdxΪ����ʱ��һ���Խ�����contours����
	//thickness Thickness of lines the contours are drawn with. If it is negative (for example,
	//thickness = #FILLED ), the contour interiors are drawn.
	//thickness ����ʱ�����

	drawContours(canvas, contours, -1, Scalar(0, 0, 255), -1, LINE_8);	//һ����ȫ����
	//forѭ����������
	/*for (size_t i = 0;i < contours.size();i++)
	{
		drawContours(canvas, contours, i, Scalar(0, 0, 255), -1, LINE_8);
	}*/
	imshow("�����", canvas);
}

const char* winMouseDemo = "Mouse Demo";
Mat temp;
Point sp(-1, -1), ep(-1, -1);
static void onMouse(int event, int x, int y, int flags, void* userdata)
{
	Mat image = *((Mat*)userdata);
	int dx, dy;
	Mat roiImg;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		sp.x = x;
		sp.y = y;
		printf("Start Point: (%i,%i)\n", x, y);
		break;
	case EVENT_LBUTTONUP:
		ep.x = x;
		ep.y = y;
		dx = ep.x - sp.x;
		dy = ep.y - sp.y;
		if (dx > 0 && dy > 0)
		{
			Rect box(sp.x, sp.y, dx, dy);
			temp.copyTo(image);	//ˢ��
			imshow("ROI", image(box));
			rectangle(image, box, Scalar(0, 0, 255), 2, LINE_AA, 0);
			imshow(winMouseDemo, image);
			sp.x = -1, sp.y = -1;	//Ϊ�´λ�ͼ����ֵstart point
		}
		break;
	case EVENT_MOUSEMOVE:
		if (sp.x > 0 && sp.y > 0)	//��갴��֮������Ч
		{
			ep.x = x;
			ep.y = y;
			dx = ep.x - sp.x;
			dy = ep.y - sp.y;
			if (dx > 0 && dy > 0)
			{
				Rect box(sp.x, sp.y, dx, dy);
				temp.copyTo(image);	//ˢ��
				rectangle(image, box, Scalar(0, 0, 255), 2, LINE_AA, 0);
				imshow(winMouseDemo, image);
			}
		}
		break;
	default:
		break;
	}
}
void QuickDemo::mouse_drawing_demo(Mat& image)
{
	namedWindow(winMouseDemo, WINDOW_AUTOSIZE);
	setMouseCallback(winMouseDemo, onMouse, (void*)(&image));
	imshow(winMouseDemo, image);
	temp = image.clone();	//����ԭͼ��Ϊ�˻�ͼˢ��
}

void QuickDemo::norm_demo(Mat& image)
{
	Mat dst;
	cout << image.type() << endl;
	//print(image);
	//int iTest = image.at<char>(20, 20);
	image.convertTo(image, CV_32F);	//��CV_8UC3ת��CV_32FC3, 32λ�����ͣ�3ͨ��ͼƬ
	//float test = image.at<float>(20, 20);
	cout << image.type() << endl;
	imshow("CV_32F", image);

	//��һ��
	normalize(image, dst, 1.0, 0.0, NORM_MINMAX);
	//test = dst.at<float>(20, 20);
	cout << dst.type() << endl;
	//print(dst);

	imshow("ͼ�����ݹ�һ��", dst);
}

void QuickDemo::resize_demo(Mat& image)
{
	Mat zoomin, zoomout;
	int w = image.cols;
	int h = image.rows;
	resize(image, zoomin, Size(w / 2, h / 2), 0, 0, INTER_LINEAR);
	imshow("zoomin", zoomin);

	resize(image, zoomout, Size(w * 1.5, h * 1.5), 0, 0, INTER_LINEAR);
	imshow("zoomout", zoomout);
}

void QuickDemo::flip_demo(Mat& image)
{
	Mat dst;
	// flip(image, dst, 0); // ���·�ת
	// flip(image, dst, 1); // ���ҷ�ת
	flip(image, dst, -1); // 180����ת
	imshow("ͼ��ת", dst);
}

void QuickDemo::rotate_demo(Mat& image)
{
	Mat dst;
	Mat M;	//��ת����

	//rotate(image, dst, ROTATE_90_CLOCKWISE);
	//imshow("rotate", dst);

	//https://cloud.tencent.com/developer/article/1798209
	int W = image.cols;
	int H = image.rows;

	M = getRotationMatrix2D(Point2f(W / 2, H / 2), 45, 1);	// ���ĵ㣬��ת�Ƕȣ� ���ű���
	//ֱ��������ת�ᶪʧͼƬ��Ϣ
	//warpAffine(image, dst, M, image.size());

	//������Ҫ���󻭲�
	//Ĭ��M
	//		 -			 -
	//	M=  | cos  sin  0 | 
	//		| -sin cos  0 |
	//		 -           -
	//�ο�ͼRotate_M.png
	double cos = abs(M.at<double>(0, 0));
	double sin = abs(M.at<double>(0, 1));
	////�ο�ͼRotate_M1.png
	int new_W = W * cos + H * sin;
	int new_H = H * cos + W * sin;
	M.at<double>(0, 2) += (new_W - W) / 2;	//cx, x����ƫ��,   ��Ҫ�������Ļ���ƽ�Ƶ�ԭ���ĵط�
	M.at<double>(1, 2) += (new_H - H) / 2;	//cy, y����ƫ��
	warpAffine(image, dst, M, Size(new_W, new_H), INTER_LINEAR, 0, Scalar(255, 0, 0));
	imshow("rotate demo", dst);
}

void QuickDemo::video_demo()
{
	//VideoCapture capture(0);
	const char* srcVideo = "C:/images/bike.avi";
	VideoCapture capture(srcVideo);
	if (!capture.isOpened())
	{
		cout << "could not open the input video" << srcVideo << endl;
	}
	int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);
	int frame_count = capture.get(CAP_PROP_FRAME_COUNT);
	double fps = capture.get(CAP_PROP_FPS);
	cout << "width: " << frame_width << endl;
	cout << "heigh: " << frame_height << endl;
	cout << "count: " << frame_count << endl;
	cout << "fps: " << fps << endl;

	int ex = static_cast<int>(capture.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form
	// Transform from int to char via Bitwise operators
	char EXT[] = { (char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0 };
	cout << "Input codec type: " << EXT << endl;

	Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)capture.get(CAP_PROP_FRAME_HEIGHT));

	const char* outputFile = "C:/images/02_writer.mp4";
	//VideoWriter writer;	
	//bool bOpen = writer.open(outputFile, ex, fps, S, true);
	VideoWriter writer(outputFile, capture.get(VideoCaptureProperties::CAP_PROP_FOURCC), fps, Size(frame_width, frame_height), true);
	if (!writer.isOpened())
	{
		cout << "Could not open the output video for write: " << outputFile << endl;
		return;
	}

	Mat frame;
	while (true)
	{
		//capture.read(frame);
		capture >> frame;
		if (frame.empty())
			break;

		writer.write(frame);
		//writer << frame;

		//imshow("video demo", frame);


		int c = waitKey(1);
		if (c == 27)
		{
			break;
		}
	}
	capture.release();
	writer.release();
}

void QuickDemo::histogram_demo(Mat& image)
{
	// ��ͨ������
	std::vector<Mat> bgr_plane;
	split(image, bgr_plane);
	// �����������
	const int channels[1] = { 0 };
	const int bins[1] = { 256 };
	float hranges[2] = { 0,255 };
	const float* ranges[1] = { hranges };
	Mat b_hist;
	Mat g_hist;
	Mat r_hist;
	// ����Blue, Green, Redͨ����ֱ��ͼ
	calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
	calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
	calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);

	// ��ʾֱ��ͼ
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / bins[0]);
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
	// ��һ��ֱ��ͼ����
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	// ����ֱ��ͼ����
	for (int i = 1; i < bins[0]; i++) {
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	// ��ʾֱ��ͼ
	namedWindow("Histogram Demo", WINDOW_AUTOSIZE);
	imshow("Histogram Demo", histImage);
}

void QuickDemo::histogram_2d_demo(Mat& image)
{
	// 2D ֱ��ͼ
	Mat hsv, hs_hist;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	int hbins = 30, sbins = 32;
	int hist_bins[] = { hbins, sbins };
	float h_range[] = { 0, 180 };
	float s_range[] = { 0, 256 };
	const float* hs_ranges[] = { h_range, s_range };
	int hs_channels[] = { 0, 1 };
	calcHist(&hsv, 1, hs_channels, Mat(), hs_hist, 2, hist_bins, hs_ranges, true, false);
	double maxVal = 0;
	minMaxLoc(hs_hist, 0, &maxVal, 0, 0);
	int scale = 10;
	Mat hist2d_image = Mat::zeros(sbins*scale, hbins * scale, CV_8UC3);
	for (int h = 0; h < hbins; h++) {
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hs_hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(hist2d_image, Point(h*scale, s*scale),
				Point((h + 1)*scale - 1, (s + 1)*scale - 1),
				Scalar::all(intensity),
				-1);
		}
	}
	applyColorMap(hist2d_image, hist2d_image, COLORMAP_JET);
	imshow("H-S Histogram", hist2d_image);
	imwrite("D:/hist_2d.png", hist2d_image);
}

void QuickDemo::histogram_eq_demo(Mat& image)
{
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("�Ҷ�ͼ��", gray);
	Mat dst;
	equalizeHist(gray, dst);
	imshow("ֱ��ͼ���⻯��ʾ", dst);
}

void QuickDemo::blur_demo(Mat& image)
{
	Mat dst;
	blur(image, dst, Size(15, 15), Point(-1, -1), BORDER_DEFAULT);
	imshow("blur", dst);
}

void QuickDemo::gaussian_blur_demo(Mat& image)
{
	Mat dst;
	//GaussianBlur(image, dst, Size(0, 0), 5);
	GaussianBlur(image, dst, Size(15, 15), 10);
	imshow("��˹ģ��", dst);
}

void QuickDemo::bifilter_demo(Mat& image)
{
	Mat dst;
	bilateralFilter(image, dst, 0, 100, 15, BORDER_DEFAULT);
	imshow("˫��ģ��", dst);
}

void QuickDemo::face_detection_demo()
{
	string root_dir = "C:/opencv-4.5.1/sources/samples/dnn/face_detector/";
	string model = root_dir + "opencv_face_detector_uint8.pb";	//ģ��
	string config = root_dir + "opencv_face_detector.pbtxt";
	dnn::Net net = dnn::readNetFromTensorflow(model, config);
	//VideoCapture capture("C:/images/example_dsh.mp4");
	VideoCapture capture(0);
	Mat frame;
	while (true) 
	{
		capture.read(frame);
		if (frame.empty())
			break;
		//������
		//Meanֵ��models.yml�в鿴 -> mean: [104, 177, 123]
		//model: "opencv_face_detector.caffemodel"
		//config : "opencv_face_detector.prototxt"
		//mean : [104, 177, 123]
		//scale : 1.0
		//width : 300
		//height : 300
		//rgb : false
		//sample : "object_detection"
		Mat blob = dnn::blobFromImage(frame, 1.0, Size(300, 300), Scalar(104, 177, 123), false, false);	//��ģ��
		net.setInput(blob);// NCHW, input
		Mat probs = net.forward(); // ���� ��ȡ����
		Mat detectionMat(probs.size[2], probs.size[3], CV_32F, probs.ptr<float>());	//���
		// �������
		for (int i = 0; i < detectionMat.rows; i++) {
			float confidence = detectionMat.at<float>(i, 2);	//�÷�
			if (confidence > 0.5) {
				int x1 = static_cast<int>(detectionMat.at<float>(i, 3)*frame.cols);
				int y1 = static_cast<int>(detectionMat.at<float>(i, 4)*frame.rows);
				int x2 = static_cast<int>(detectionMat.at<float>(i, 5)*frame.cols);
				int y2 = static_cast<int>(detectionMat.at<float>(i, 6)*frame.rows);
				Rect box(x1, y1, x2 - x1, y2 - y1);
				rectangle(frame, box, Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		imshow("���������ʾ", frame);
		int c = waitKey(1);
		if (c == 27) { // �˳�
			break;
		}
	}
}
