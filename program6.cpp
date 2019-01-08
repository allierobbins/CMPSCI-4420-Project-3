/***************************************
* Program 6 for 4420 Assignment 3		   *
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
#include <string>

using namespace cv;
using namespace std;

/* Function prototypes */
bool checkIfGreyscale(Mat&);
void callBackFunc(int, int, int, int, void*);
void histogramEqualize(int (&)[256], Mat&);
void computeHistorgram(int (&)[256], Mat&);
void computeNormalizedHistogram(int (&)[256], double (&)[256], Mat&);
void runningSum(double (&)[256], double (&)[256], Mat&);
void histogram_equalized_subtract(Mat&, Mat&, Mat&, int, int, int, int) {

int main(int argc, char *argv[]) {

  /* Declare needed variables */
  int x, y, width, height;
  bool ROIProvided = false;

  /* Check to make sure a image was given */
  if (argc == 1) {
      printf("You did not provide an image. Ending program6.\n");
      exit(EXIT_FAILURE);

  /* Check if a ROI was specified in the command line */
  } else if (argc == 6) {

      /* Assign the variables according to the command line arguments */
      char * str_end;
      x = strtol(argv[2], NULL, 10);
      y = strtol(argv[3], NULL, 10);
      width = strtol(argv[4], NULL, 10);
      height = strtol(argv[5], NULL, 10);
      ROIProvided = true;

  /* If the incorrect @ of arguments were given, exit */
  } else if (argc > 6) {
      printf("You did not provide an image. Ending program6.\n");
      exit(EXIT_FAILURE);
  }

  /* Assign the input image from command line args */
  Mat image = imread(argv[1]);

  /* Make sure the images can be found/opened */
  if (image.empty()) {
    printf("There was an error opening the image. Ending program5.\n");
    exit(EXIT_FAILURE);
  }

  /* Make sure the image is in greyscale */
  Mat greyImage;
  cvtColor(image, greyImage, CV_BGR2GRAY);

  /* Declare the final image */
  Mat finalImage;

  /* Check if the image is in greyscale */
  bool grey = checkIfGreyscale(image);

  /* If the image is in greyscale */
  if (grey) {

    /* Declare the histogram array */
    int histogram[256] = {0};

    /* Compute the histogram of the grayscale image */
    computeHistorgram(histogram, greyImage);

    /* Apply histogram equalization to the grayscale image */
    histogramEqualize(histogram, greyImage);

    subtractHistogram(image, greyImage, finalImage, x, y, width, height);

    /* Create the window that will dispaly the color image */
    namedWindow("Final Image");
    imshow("Final Image", finalImage);

    /* Pause to admire images */
    waitKey(0);

  /* If the image is in color */
  } else {

        Mat RGBChannels[3];
        Mat finalRGB[3];
        Mat originalRGB[3];

        /* Use the OpenCV funciton split() to get the RGB values */
        split(image, RGBChannels);
        split(image, finalRGB);
        split(image, originalRGB);

        /* Array to hold the RGB histogram values */
        int RGBhistogram[3][256] ={0};

        /* Compute the histograms for each color channel */
        computeHistorgram(RGBhistogram[0], RGBChannels[0]);
        computeHistorgram(RGBhistogram[1], RGBChannels[1]);
        computeHistorgram(RGBhistogram[2], RGBChannels[2]);

        /* Apply equalization to each color channel */
        histogramEqualize(RGBhistogram[0], RGBChannels[0]);
        histogramEqualize(RGBhistogram[1], RGBChannels[1]);
        histogramEqualize(RGBhistogram[2], RGBChannels[2]);

        /* Subtract the original image from the histogram equalized image */
        subtractHistogram(originalRGB[0], RGBChannels[0], finalRGB[0], x, y, width, height);
        subtractHistogram(originalRGB[1], RGBChannels[1], finalRGB[1], x, y, width, height);
        subtractHistogram(originalRGB[2], RGBChannels[2], finalRGB[2], x, y, width, height);

        /* Merge the histogram equalizationed ROI to the final image */
        merge(finalRGB, 3, finalImage);

        /* Create the window that will dispaly the color image */
        namedWindow("Final Image");
        imshow("Final Image", finalImage);

        /* Pause to admire images */
        waitKey(0);

  }

  /* End of program6 */
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

void histogram_equalized_subtract(Mat &image, Mat &greyImage, Mat &finalImage, int x, int y, int width, int height) {

    int cMax, rMax, int cStart, int rStart;

    /* If the image size not given the beginning is the origin */
    if ((width == 0) && (height == 0) && (x == 0) && (y == 0)) {
      cMax = img.cols;
      rMax = img.rows;
      cStart = 0;
      rStart = 0;
    /* Otherwise it is as specifed */
    } else {
      cMax = x + width;
      rMax = y + height;
      cStart = x;
      rStart = y;
    }

    /* Loop through the image and compute the final image */
    for (int i = rStart; i < rMax; i++) {
      for (int j = cStart; j < cMax; j++) {

        int value = (abs(greyImage.at<uchar>(i, j) - img.at<uchar>(i, j)));

        finalImage.at<uchar>(i, j) = value;
      }
    }
}



/********************* Internet Sources ******************
https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
https://htrd.su/wiki/_media/zhurnal/2013/01/28/opencv_cheatsheet.pdf
*/
