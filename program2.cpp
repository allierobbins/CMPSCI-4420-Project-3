/***************************************
* Program 2 for 4420 Assignment 3		   *
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
void thresholdTheImage(double, Mat&);
double findMean(Mat image);

int main(int argc, char *argv[]) {

  /* Check to make sure a image was given */
  if (argc == 1) {
    printf("You did not provide an image. Ending program2.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the input image from command line args */
  Mat image = imread(argv[1]);

  /* Make sure the images can be found/opened */
  if (image.empty()) {
    printf("There was an error opening the image. Ending program2.\n");
    exit(EXIT_FAILURE);
  }

  /* Check if the image is in greyscale */
  bool grey = checkIfGreyscale(image);

  /* Declare the image that will hold the merged images */
  Mat mergedImage;

  /* If the image is in greyscale */
  if (grey) {

        /* Get the mean of the grayscale image */
        double greyMean = findMean(image);

        /* Threshold the grayscale image */
        thresholdTheImage(greyMean, mergedImage);

        /* Display the image to the screen */
        namedWindow("Input Image");
        imshow("Input Image", mergedImage);

    /* If the image is in color */
  } else {

        /* Use the OpenCV funciton split() to get the RGB values */
        Mat RGBChannels[3];
        split(image, RGBChannels);

        /* Find the mean of each RGB values */
        double blueMean = findMean(RGBChannels[0]);
        double greenMean = findMean(RGBChannels[1]);
        double redMean = findMean(RGBChannels[2]);

        /* Threshold each RGB mean value */
        thresholdTheImage(blueMean, RGBChannels[0]);
        thresholdTheImage(greenMean, RGBChannels[1]);
        thresholdTheImage(redMean, RGBChannels[2]);

        /* Merge the thresholded values together and store in mergedImage */
        merge(RGBChannels, 3, mergedImage);

        /* Display the image to the screen */
        namedWindow("Input Image");
        imshow("Input Image", mergedImage);

  }

  /* Pause to admire image */
  waitKey(0);

  /* End of program2 */
  return 0;
}

/* Function to determine if an image is in greyscale - based on code located on Internet source #1 */
bool checkIfGreyscale(Mat &image) {

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

/* Function to threshold a given image - based on Algoritm 10.3 from textbook */
void thresholdTheImage(double mean, Mat &image) {

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {

      /* Get the pixel value @ a given point */
      int value = image.at<uchar>(i, j);

      /* If the pixel value is below the mean, set the value to black */
      if (value < (int)mean)
        image.at<uchar>(i, j) = 0;
        /* Else, set the value to white */
      else
        image.at<uchar>(i, j) = 255;
    }
  }
} /* End of thresholdTheImage function */

/* Function taken from Assignment 2 to calculate mean */
double findMean(Mat image) {

  int sum;

  /* Loop through the values and add the sum*/
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      sum += image.at<uchar>(i, j);
    }
  }

  /* mean = sum/# of pixels */
  double mean = (double)(sum) / (double)(image.rows * image.cols);

  /* Return the mean value */
  return mean;

} /* End of findMean function */


/*************Internet Sources*************
2. https://stackoverflow.com/questions/23660929/how-to-check-whether-a-jpeg-image-is-color-or-gray-scale-using-only-python-stdli
3. http://answers.opencv.org/question/36288/how-to-know-grayscale-or-color-img-loaded/
4. http://opencvexamples.blogspot.com/2013/10/basic-drawing-examples.html
******************************************/
