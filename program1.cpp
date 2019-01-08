/***************************************
* Program 1 for 4420 Assignment 3		   *
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
void callBackFunc(int, int, int, int, void*);
bool checkIfGreyscale(Mat&);
string convertToString(int);

int main(int argc, char *argv[]) {

  /* Check to make sure a image was given */
  if (argc == 1) {
    printf("You did not provide an image. Ending program1.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the input image from command line args */
  Mat image = imread(argv[1]);

  /* Make sure the images can be found/opened */
  if (image.empty()) {
    printf("There was an error opening the image. Ending program1.\n");
    exit(EXIT_FAILURE);
  }

  /* Create the window that will dispaly the image */
  namedWindow("Clickable Image");

  /* When the mouse is clicked - call callBackFunc */
  setMouseCallback("Clickable Image", callBackFunc, &image);

  /* Display the image to the screen */
  imshow("Clickable Image", image);

  /* Pause to admire image */
  waitKey(0);

  /* End of program1 */
  return 0;
}

/* Function that is called when the mouse is clicked - based off same function found on Internet source 1*/
void callBackFunc(int event, int x, int y, int flags, void *userdata) {

  Mat *image = (Mat*) userdata;

  /* If the left mouse button is clicked */
  if (event == EVENT_LBUTTONDOWN) {

    /* Check if the image is in greyscale */
    bool grey = checkIfGreyscale((*image));

    /* If the image is in greyscale */
    if (grey) {

      /* Assign clicked pixel to variable */
      int pixel = (*image).at<uchar>(y, x);

      /* Create string for the value */
      string text = convertToString(pixel);

      /* position is the selected pixel location */
      Point position = Point(x, y);

      /* Use OpenCV function getTextSize to calculate the width & height of the string*/
      Size size = getTextSize(text, FONT_HERSHEY_COMPLEX_SMALL, 1, 1, 0);

      /* Assign the length of the text box */
      Point start = Point(x, y);
      Point stop = Point(x + size.width, y - size.height);

      /* Draw the rectangle onto the image */
      rectangle((*image), start, stop, CV_RGB(0, 0, 0), CV_FILLED);

      /* Use OpenCV function putText to draw the string */
      putText((*image), text, position, FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255), 1, 1);

      /* Print the intensity value - cannot use printf (generates lots o errors) */
      cout << "The greyscale pixel value is " << text << ".\n";

      //Show the image with the text added to it.
      imshow("Clickable Image", (*image));

      /* If the image is in color */
    } else {

      /* variable to seperate the RGB values */
      Vec3b intensity = (*image).at<Vec3b>(y, x);

      /* Get the RGB values at the position */
      int blue_pixel = intensity.val[0];
      int green_pixel = intensity.val[1];
      int red_pixel = intensity.val[2];

      /* Create string with the intensity values seperated by a , */
      string text = convertToString(blue_pixel);
      text.append(", ");
      text.append(convertToString(green_pixel));
      text.append(", ");
      text.append(convertToString(red_pixel));

      /* Print the intensity value - cannot use printf (generates lots o errors) */
      cout << "The RGB pixel values are " << text << ".\n";

      /* position is the selected pixel location */
      Point position = Point(x, y);

      /* Use OpenCV function getTextSize to calculate the width & height of the string*/
      Size size = getTextSize(text, FONT_HERSHEY_COMPLEX, 1, 1, 0);

      /* Assign the length of the text box */
      Point start = Point(x, y);
      Point stop = Point(x + size.width, y - size.height);

      /* Draw the rectangle onto the image */
      rectangle((*image), start, stop, CV_RGB(0, 0, 0), CV_FILLED);

      /* Use OpenCV function putText to draw the string */
      putText((*image), text, position, FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 1, 1);

      //Show the image with the text added to it.
      imshow("Clickable Image", (*image));
    }
  }
} /* End of callBackFunc function */

/* Function to determine if an image is in greyscale - based on code located on Internet source #2 */
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

/* Function used to convert the individual datas into a string */
string convertToString(int pixel) {
  string str;
  stringstream str_stream;
  str_stream << pixel;
  str.assign(str_stream.str());
  return str;
} /* End of convertToString function */

/*************Internet Sources*************
1. http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
2. https://stackoverflow.com/questions/23660929/how-to-check-whether-a-jpeg-image-is-color-or-gray-scale-using-only-python-stdli
3. http://answers.opencv.org/question/36288/how-to-know-grayscale-or-color-img-loaded/
4. http://opencvexamples.blogspot.com/2013/10/basic-drawing-examples.html
******************************************/
