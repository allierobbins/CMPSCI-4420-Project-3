/***************************************
* Program 4 for 4420 Assignment 3		   *
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


  return 0;
}
