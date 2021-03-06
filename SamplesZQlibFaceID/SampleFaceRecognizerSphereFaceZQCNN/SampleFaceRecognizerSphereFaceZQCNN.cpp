#include "ZQ_FaceRecognizerSphereFaceZQCNN.h"
#include <cblas.h>
using namespace ZQ;
using namespace cv;
using namespace std;

int main()
{
	openblas_set_num_threads(1);
	ZQ_FaceRecognizer* recognizer[3] = { 0 };
	std::string model_name[3] = {
		"04bn256","06bn512","mobile-10bn512"
	};

	bool fail_flag = false;
	for (int i = 0; i < 3; i++)
	{
		recognizer[i] = new ZQ_FaceRecognizerSphereFaceZQCNN();
		if (!recognizer[i]->Init(model_name[i]))
		{
			cout << "failed to init " << model_name[i] << " \n";
			fail_flag = true;
			break;
		}
	}
	if (fail_flag)
	{
		for (int i = 0; i < 3; i++)
		{
			if (recognizer[i]) delete recognizer[i];
		}
		return EXIT_FAILURE;
	}

	Mat img0 = imread("data\\00.jpg");
	Mat img1 = imread("data\\01.jpg");

	for (int it = 0; it < 10; it++)
	{

		for (int i = 0; i < 3; i++)
		{
			int feat_dim = recognizer[i]->GetFeatDim();
			std::vector<float> feat0(feat_dim), feat1(feat_dim);
			recognizer[i]->ExtractFeature(img0.data, img0.step[0], ZQ_PIXEL_FMT_BGR, &feat0[0], true);
			recognizer[i]->ExtractFeature(img1.data, img1.step[0], ZQ_PIXEL_FMT_BGR, &feat1[0], true);
			float score = recognizer[i]->CalSimilarity(&feat0[0], &feat1[0]);
			cout << "model " << model_name[i] << " gets score " << score << "\n";
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (recognizer[i]) delete recognizer[i];
	}
	return EXIT_SUCCESS;
}