#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

 
void       get_random_particles(Mat &img, vector<MatND> &ary) {

	int                 width               = img.cols / 20;
	int                 height              = img.rows / 20;
	int                 numbins             = 255;
	const    int        channel[]           = { 0,1,2 };
	float               channel_range[]     = { 0.0, 255.0 };
	const    float*     channel_ranges      = channel_range;

	for (int i = 0;	i < 2000; i++)
    {
            int             x                   = rand() % (img.cols-width-1);
            int             y                   = rand() % (img.rows-height-1);
            Rect            rect(x, y, width, height);
            Mat             part                = img(rect);
            MatND           b_histo;
            MatND           g_histo;
            MatND           r_histo;
            vector<Mat>     bgr;

            split(part, bgr);

            calcHist(&bgr[0], 1, 0, Mat(), b_histo, 1, &numbins, &channel_ranges);
            calcHist(&bgr[0], 1, 0, Mat(), g_histo, 1, &numbins, &channel_ranges);
            calcHist(&bgr[0], 1, 0, Mat(), r_histo, 1, &numbins, &channel_ranges);

            ary.push_back(b_histo);
            ary.push_back(g_histo);
            ary.push_back(r_histo);
	}
}

double      get_distance(vector<MatND> &ori, vector<MatND> &comp)
{
	double      sum_of_distance      = 0.0;

	for (int i = 0; i < ori.size(); i++) 
	{
		sum_of_distance += compareHist(ori[i], comp[i], CV_COMP_INTERSECT);
	}

	return sum_of_distance;
}

int			main(int argc, const char** argv)
{
	// Read images
	Mat             mountain            = imread("./mountain.jpg");	
	Mat             sky                 = imread("./sky.jpg");
	Mat             desert              = imread("./desert.jpg");
	Mat             compare             = imread("./compare.jpg");
	vector<MatND>   mountain_image;
	vector<MatND>   sky_image;
	vector<MatND>   desert_image;
	vector<MatND>   compared_image;
	// Get same pixel with opencv function
	get_random_particles(mountain, mountain_image);
	get_random_particles(sky, sky_image);
	get_random_particles(desert, desert_image);
	get_random_particles(compare, compared_image);

	double          sim_mnt             = get_distance(mountain_image, compared_image);
	double          sim_sky             = get_distance(sky_image, compared_image);
	double          sim_dsrt            = get_distance(desert_image, compared_image);
	double          sim_max             = MAX(sim_dsrt,MAX(sim_mnt, sim_sky));

	if (sim_max == sim_dsrt) {
		printf("가장 유사도 높은 사진은 사막입니다.");

	}
	else if (sim_max == sim_mnt) {
		printf("가장 유사도 높은 사진은 산입니다.");
	}
	else {
		printf("가장 유사도 높은 사진은 하늘입니다.");
	}

	waitKey(0);

	return 0;
}
