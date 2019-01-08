/***************************************
* Program 5 for 4420 Assignment 3		   *
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

/* Global variables to mark the beginning/end of the ROI */
Point start;
Point stop;

/* Function prototypes */
bool checkIfGreyscale(Mat&);
void callBackFunc(int, int, int, int, void*);
void histogramEqualize(int (&)[256], Mat&, Rect);
void computeHistorgram(int (&)[256], Mat&);
void computeNormalizedHistogram(int (&)[256], double (&)[256], Mat&);
void runningSum(double (&)[256], double (&)[256], Mat&);

int main(int argc, char *argv[]) {

  /* Check to make sure a image was given */
  if (argc == 1) {
    printf("You did not provide an image. Ending program5.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the input image from command line args */
  Mat image = imread(argv[1]);

  /* Make sure the images can be found/opened */
  if (image.empty()) {
    printf("There was an error opening the image. Ending program5.\n");
    exit(EXIT_FAILURE);
  }

  /* Create the window to select a ROI via mouse input */
  namedWindow("Prompt Window");
  setMouseCallback("Prompt Window", callBackFunc, NULL);
  imshow("Prompt Window", image);

  waitKey(0);

  /* Check if the image is in greyscale */
  bool grey = checkIfGreyscale(image);

  /* Convert the ROI to greyscale and store in ROIImage variable */
  Mat ROIImage;
  cvtColor(image, ROIImage, CV_BGR2GRAY);

  /* Implement the ROI as a Rect with the given start/stop positions */
  Rect roi = Rect(start, stop);

  /* If the image is in greyscale */
  if (grey) {

      /* Declare the histogram array */
      int histogram[256] = {0};

      /* Compute the histogram of the grayscale image */
      computeHistorgram(histogram, ROIImage);

      /* Apply histogram equalization to the grayscale image */
      histogramEqualize(histogram, ROIImage, roi);

  /* If the image is in color */
  } else {

      /* Use the OpenCV funciton split() to get the RGB values */
      Mat RGBChannels[3];
      split(image, RGBChannels);

      /* Array to hold the RGB histogram values */
      int RGBhistogram[3][256] = {0};

      /* Compute the histograms for each color channel */
      computeHistorgram(RGBhistogram[0], RGBChannels[0]);
      computeHistorgram(RGBhistogram[1], RGBChannels[1]);
      computeHistorgram(RGBhistogram[2], RGBChannels[2]);

      /* Apply equalization to each color channel of the ROI */
      histogramEqualize(RGBhistogram[0], RGBChannels[0], roi);
      histogramEqualize(RGBhistogram[1], RGBChannels[0], roi);
      histogramEqualize(RGBhistogram[2], RGBChannels[0], roi);

      /* Merge the histogram equalizationed ROI and the final image */
      merge(RGBChannels, 3, ROIImage);

  }

  /* Create the window that will dispaly the color image */
  namedWindow("Final Image");
  imshow("Final Image", ROIImage);

  /* Pause to admire images */
  waitKey(0);

  /* End of program5 */
  return 0;

}

//The mouse event handle for the user mouse click.
void callBackFunc(int event, int x, int y, int flags, void *data) {

  /* When the mouse is clicked/unlclicked, the position is marked */
  if (event == EVENT_LBUTTONDOWN) {
    start.x = x;
    start.y = y;
  } else if (event == EVENT_LBUTTONUP) {
    stop.x = x;
    stop.y = y;
  }

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
