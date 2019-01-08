/***************************************
* Program 3 for 4420 Assignment 3		   *
* Author: Allie Robbins								 *
* Language: C++ with OpenCV libraries	 *
* IDE: Visual Studio 2017							 *
***************************************/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;

/* Function prototypes */
bool checkIfGreyscale(Mat&);
void histogramEqualize(int (&)[256], Mat&, Rect);
void computeHistorgram(int (&)[256], Mat&);
void computeNormalizedHistogram(int (&)[256], double (&)[256], Mat&);
void runningSum(double (&)[256], double (&)[256], Mat&);

int main(int argc, char *argv[]) {

  /* Check to make sure a image was given */
  if (argc == 1) {
    printf("You did not provide an image. Ending program3.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the input image from command line args */
  Mat image = imread(argv[1]);

  /* Make sure the images can be found/opened */
  if (image.empty()) {
    printf("There was an error opening the image. Ending program3.\n");
    exit(EXIT_FAILURE);
  }

  /* Check if the image is in greyscale */
  bool grey = checkIfGreyscale(image);

  /* If the image is a greyscale image */
  if (grey) {

    /* Declare the histogram arrays */
    int histogram[256] = {0};

    /* Compute the histogram of the grayscale image */
    computeHistorgram(histogram, image);

    /* Apply histogram equalization to the grayscale image */
    histogramEqualize(histogram, image);

    /* Create the window that will dispaly the greyscale image */
    namedWindow("Histogram equalization in grayscale");
    imshow("Histogram equalization in grayscale", image);

  } else {

    /* Declare the color image to store seperated RGB */
    Mat colorImage;

    /* Use the OpenCV funciton split() to get the RGB values */
    Mat RGBChannels[3];
    split(image, RGBChannels);

    /* Array to hold the RGB histogram values */
    int RGBhistogram[3][256] = {0};

    /* Compute the histograms for each color channel */
    computeHistorgram(RGBhistogram[0], RGBChannels[0]);
    computeHistorgram(RGBhistogram[1], RGBChannels[1]);
    computeHistorgram(RGBhistogram[2], RGBChannels[2]);

    /* Apply equalization to each color channel */
    histogramEqualize(RGBhistogram[0], RGBChannels[0]);
    histogramEqualize(RGBhistogram[1], RGBChannels[1]);
    histogramEqualize(RGBhistogram[2], RGBChannels[2]);

    /* Use the OpenCV function to merge the RGB colors to the equalized image */
    merge(RGBChannels, 3, colorImage);

    /* Create the window that will dispaly the color image */
    namedWindow("Histogram equalization in color");
    imshow("Histogram equalization in color", colorImage);

  }

  /* Pause to admire images */
  waitKey(0);

  /* End of program3 */
  return 0;
}

/* Function to determine if an image is in greyscale - based on code located on Internet source #1 */
bool checkIfGreyscale(Mat &image) {

  /* Use the OpenCV funciton split() to get the RGB values */
  Mat RGBChannels[3];
  split(image, RGBChannels);

  /* Loop through the image and compare the RGB values  */
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {

      int blue = RGBChannels[0].at<uchar>(i, j);
      int green = RGBChannels[1].at<uchar>(i, j);
      int red = RGBChannels[2].at<uchar>(i, j);

      /* If the RGB values are different, the image is not greyscale */
      if ((blue != green) && (blue != red))
        return false;
    }
  }
  /* If reached, the image IS greyscale */
  return true;

} /* End of checkIfGreyscale function */

/* Function to compute the greyscale histogram PDF - taken from Algorithm 3.7 from the textbook */
void computeHistorgram(int (&histogram)[256], Mat &image) {

  /* Loop through the array and calculate the histogram */
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      histogram[(int)image.at<uchar>(i, j)] += 1;
    }
  }
} /* End of computeHistorgram function */

/* Function to perform histogram equalization - based on Algorithm 3.9 from the textbook */
void histogramEqualize(int (&histogram)[256], Mat &image, Rect rect) {

  /* Declare needed variables */
  int row, col, rMax, cMax;
  double normalizedHistogram[256] = {0.0};
  double cdf[256] = {0.0};

  /* Compute the normalized hisogram */
  computeNormalizedHistogram(histogram, normalizedHistogram, image);

  /* Compute the CDF of the grayscale image */
  runningSum(normalizedHistogram, cdf, image);

  if ((rect.x != -1) && (rect.y != -1)) {
    row = rect.y;
    rMax = rect.y + rect.height;
    col = rect.x;
    cMax = rect.x + rect.width;
  } else {
    row = 0;
    col = 0;
    rMax = image.rows;
    cMax = image.cols;
  }

  for (int i = row; i < rMax; i++) {
    for (int j = col; j < cMax; j++) {
      image.at<uchar>(i, j) =
        round(255 * cdf[image.at<uchar>(i, j)]);
    }
  }
} /* End of histogramEqualize function */

/* Function to compute the normalized histogram - taken from Algoritm 3.8 from the textbook */
void computeNormalizedHistogram(int (&histogram)[256], double (&normalizedHistogram)[256], Mat &image) {

  int total = (image.size().height) * (image.size().width);

  for (int i = 0; i < 256; i++) {
    normalizedHistogram[i] = (((double)histogram[i]) / ((double)(total)));
  }
} /* End of computeNormalizedHistogram function */

/* Function to perform histogram equalization - taken from Algoritm 3.9 from the textbook */
void runningSum(double (&normalizedHistogram)[256], double (&CDF)[256], Mat &image) {

  double sum = 0.0;

  /* Loop through the normalized array and calculate the running sum*/
  for (int i = 0; i < 256; i++) {
    sum += normalizedHistogram[i];
    CDF[i] = sum;
  }
} /* End of runningSum function */

/**************** Internet sources ****************
1. https://www.opencv-srf.com/2018/02/histogram-equalization.html
2. https://stackoverflow.com/questions/15007304/histogram-equalization-not-working-on-color-image-opencv
3. https://www.daniweb.com/programming/software-development/threads/310892/simple-running-total-c
*/
