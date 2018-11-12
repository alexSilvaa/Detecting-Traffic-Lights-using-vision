#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <conio.h> 
#include <algorithm>

using namespace std;
using namespace cv;

int im1L1 = 319, im1L2 = 692;
int im2L1 = 217, im2L2 = 794;        //groundtruth for columns, used to assert
int im3L1 = 347, im3L2 = 701;        //whether its a match for light 1 or 2
int im4L1 = 271, im4L2 = 640;        //and also to find the best match
int im5L1 = 261, im5L2 = 644;
int im6L1 = 238, im6L2 = 650;
int im7L1 = 307, im7L2 = 747;
int im8L1 = 280, im8L2 = 795;
int im9L1 = 359, im9L2 = 630;
int im10L1 = 233, im10L2 = 681;
int im11L1 = 331, im11L2 = 663;
int im12L1 = 373, im12L2 = 715;
int im13L1 = 272, im13L2 = 604;
int im14L1 = 279, im14L2 = 719;
                           //ground truth for states of lights
//green
int one1 = 0, one2 = 0, two1 = 0, two2 = 0, three1 = 0, three2 = 0, six1 = 0, six2 = 0, nine1 = 0, nine2 = 0, ten1 = 0, ten2 = 0, leven1 = 0, leven2 = 0, twelve1 = 0, twelve2 = 0;
//yellow
int sev1 = 1, sev2 = 1, eight1 = 1, eight2 = 1;
//red
int four1 = 2, four2 = 2, thirt1 = 2, thirt2 = 2, fourt1 = 2, fourt2 = 2;
//red + yellow
int five1 = 3, five2 = 3;

double match(string imgName, string tempName) { //takes in image and single template
	Mat srcImg, tempImg;                        
	Mat result;
	Mat debug;
	

	tempImg = imread(tempName);      
	if (tempImg.data == NULL) {
		printf("template imread() failed...\n");
		
	}

	srcImg = imread(imgName);     
	if (srcImg.data == NULL) {
		printf("image imread() failed...\n");
		
	}
	debug = srcImg;
	double minVal = 0; double maxVal = 0;
	while (true) {  
		
		int match_method = CV_TM_CCORR_NORMED;
		matchTemplate(srcImg, tempImg, result, match_method);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());   //if i dont do this matches become worse 

		Point minLoc, maxLoc, matchLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)  matchLoc = minLoc;
		else matchLoc = maxLoc;
		
		rectangle(
			debug,
			matchLoc,
			Point(matchLoc.x + (tempImg.cols), matchLoc.y + (tempImg.rows/1.5)),
			CV_RGB(255, 0, 0),
			3);

		
		//imshow("debug image", debug);
		//waitKey(100);
		//system("Pause");
		break;
	}

	return minVal*1000000;      //initial value is very small
}

Point matchCol(string imgName, string tempName) {  //returns location of the match
	Mat srcImg, tempImg;
	Mat result;
	Mat debug;


	tempImg = imread(tempName);       
	if (tempImg.data == NULL) {
		printf("template imread() failed...\n");

	}

	srcImg = imread(imgName);      
	if (srcImg.data == NULL) {
		printf("image imread() failed...\n");

	}
	
	debug = srcImg;
	Point minLoc, maxLoc, matchLoc;
	double minVal = 0; double maxVal = 0;

	while (true) {

		int match_method = CV_TM_CCORR_NORMED;
		matchTemplate(srcImg, tempImg, result, match_method);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)  matchLoc = minLoc;
		else matchLoc = maxLoc;

		rectangle(
			debug,
			matchLoc,
			Point(matchLoc.x + tempImg.cols, matchLoc.y + tempImg.rows),
			CV_RGB(255, 0, 0),
			3);

		break;
	}

	return matchLoc;
}

Mat match2(string imgName, string tempName) { //returns cropped version of image after best match has been found 
	Mat srcImg, tempImg;
	Mat result;
	Mat debug;
	Mat cropped;

	tempImg = imread(tempName);      
	if (tempImg.data == NULL) {
		printf("template imread() failed...\n");

	}

	srcImg = imread(imgName);      
	if (srcImg.data == NULL) {
		printf("image imread() failed...\n");

	}

	Point minLoc, maxLoc, matchLoc;
	double minVal = 0; double maxVal = 0;

	while (true) {
		int match_method = CV_TM_CCORR_NORMED;
		matchTemplate(srcImg, tempImg, result, match_method);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)  matchLoc = minLoc;
		else matchLoc = maxLoc;

		
		Rect myROI(matchLoc, Point(matchLoc.x + (tempImg.cols), matchLoc.y + (tempImg.rows / 1.5)));
		cropped = srcImg(myROI);

		//imshow("cropped image", cropped);

		//waitKey(1000);

		break;
	}
	
	
	return cropped;

}

Mat getRed(Mat image) {     //returns binary image - white parts red 
	Mat labImg, result;

	cvtColor(image, labImg, CV_BGR2Lab);
	GaussianBlur(labImg, labImg, Size(5.0f, 5.0f), 20.0);

	inRange(labImg, Scalar(30.0f, 137.0f, 100.0f), Scalar(180.0f, 255.0f, 255.0f), result);
	
	//imshow("red image", result);
	//waitKey(100);
	//system("Pause");
	return result;
}

Mat getGreen(Mat image) {        //returns binary image - white parts grenn
	Mat hsvImg, result;

	cvtColor(image, hsvImg, CV_BGR2HSV);
	GaussianBlur(hsvImg, hsvImg, Size(5.0f, 5.0f), 20.0);

	inRange(hsvImg, Scalar(90.0f, 40.0f, 40.0f), Scalar(160.0f, 100.0f, 100.0f), result);

	//imshow("green image", result);
	//waitKey(100);
	//system("Pause");
	return result;
}

Mat getYellow(Mat image) {      //returns binary image - white parts yellow
	Mat hsvImg, result;

	cvtColor(image, hsvImg, CV_BGR2HSV);
	GaussianBlur(hsvImg, hsvImg, Size(5.0f, 5.0f), 20.0);

	inRange(hsvImg, Scalar(20.0f, 40.0f, 40.0f), Scalar(60.0f, 100.0f, 100.0f), result);

	//imshow("yellow image", result);
	//waitKey(100);
	//system("Pause");
	return result;
}

double getBlackPercent(Mat img) {   //returns value of black pixels
	double thresh = 0;
	int imgsize = img.rows * img.cols;
	threshold(img, img, thresh, -1, CV_THRESH_TOZERO);
	int nonzero = countNonZero(img);
	return(imgsize - nonzero) / double(imgsize);
}

double recall(int tp, int fn) {
	double r = tp + fn;
	double re = tp / r;
	return re;
}
double precision(int tp, int fp) {
	double p = tp + fp;
	double pre = tp / p;
	return pre;
}
double accuracy(int tp, int fp, int fn) {
	double a = tp + fp + fn;
	double acc = tp / a;
	return acc;
}
double dice(double p, double r) {
	double d = r * p;
	double i = r + p;
	double di = d / i;
	double dice = 2 * di;
	return dice;
}
int main(int argc, char* argv[])
{	
	int colL1, colL2;
	string temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9;
	string img1, img2, img3, img4, img5, img6, img7, img8, img9, img10, img11, img12, img13, img14;

	temp1 = "Media/t1.jpg";
	temp2 = "Media/t4.jpg";
	temp3 = "Media/t6.jpg";
	temp4 = "Media/t44.jpg";
	temp5 = "Media/t66.jpg";
	temp6 = "Media/t2.jpg";
	temp7 = "Media/tL.jpg";
	temp8 = "Media/tn1.jpg";
	temp9 = "Media/tn2.jpg";
	
	string tempArr[] = { temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9 };

	img1 = "Media/CamVidLights01.jpg";
	img2 = "Media/CamVidLights02.jpg";
	img3 = "Media/CamVidLights03.jpg";
	img4 = "Media/CamVidLights04.jpg";
	img5 = "Media/CamVidLights05.jpg";
	img6 = "Media/CamVidLights06.jpg";
	img7 = "Media/CamVidLights07.jpg";
	img8 = "Media/CamVidLights08.jpg";
	img9 = "Media/CamVidLights09.jpg";
	img10 = "Media/CamVidLights10.jpg";
	img11 = "Media/CamVidLights11.jpg";
	img12 = "Media/CamVidLights12.jpg";
	img13 = "Media/CamVidLights13.jpg";
	img14 = "Media/CamVidLights14.jpg";

	string imgarray[] = { img1, img2, img3, img4, img5, img6, img7, img8, img9, img10, img11, img12, img13, img14 };
	string imgName;
	int imageName = 0;
	int difArr1[10] = {};
	int difArr2[10] = {};
	bool exit = true;
	int FP = 0;
	int TP = 0;
	int FN = 0;
	int TN = 0;
	int FParray[14] = {};
	int countStates = 0;

	while (imageName < 14) {
		int status;
		string resultTemp1, resultTemp2;
		int col1, col2;
		Mat cimg1, cimg2;
		Mat r1, r2, g1, g2, y1, y2;
		double rb1, gb1, yb1, rb2, gb2, yb2;
		double minim1, minim2;
		Point colsLoc;
		double minval;

		//cout << "What image would you like to test?  (0-13)" << endl;
		//cin >> imageName;
		int i = imageName;
		imgName = imgarray[imageName];
		                                 //following giving the ground truth values for columns
		if (imageName == 0) {    
			colL1 = im1L1;
			colL2 = im1L2;
		}
		if (imageName == 1) {
			colL1 = im2L1;
			colL2 = im2L2;
		}
		if (imageName == 2) {
			colL1 = im3L1;
			colL2 = im3L2;
		}
		if (imageName == 3) {
			colL1 = im4L1;
			colL2 = im4L2;
		}
		if (imageName == 4) {
			colL1 = im5L1;
			colL2 = im5L2;
		}
		if (imageName == 5) {
			colL1 = im6L1;
			colL2 = im6L2;
		}
		if (imageName == 6) {
			colL1 = im7L1;
			colL2 = im7L2;
		}
		if (imageName == 7) {
			colL1 = im8L1;
			colL2 = im8L2;
		}
		if (imageName == 8) {
			colL1 = im9L1;
			colL2 = im9L2;
		}
		if (imageName == 9) {
			colL1 = im10L1;
			colL2 = im10L2;
		}
		if (imageName == 10) {
			colL1 = im11L1;
			colL2 = im11L2;
		}
		if (imageName == 11) {
			colL1 = im12L1;
			colL2 = im12L2;
		}
		if (imageName == 12) {
			colL1 = im13L1;
			colL2 = im13L2;
		}
		if (imageName == 13) {
			colL1 = im14L1;
			colL2 = im14L2;
		}
		

		int diff1_1 = 0, diff1_2 = 0, diff2_1 = 0, diff2_2 = 0, diff3_1 = 0, diff3_2 = 0, diff4_1 = 0, diff4_2 = 0;
		int diff5_1 = 0, diff5_2 = 0, diff6_1 = 0, diff6_2 = 0, diff7_1 = 0, diff7_2 = 0, diff8_1 = 0, diff8_2 = 0;
		int diff9_1 = 0, diff9_2 = 0, diff10_1 = 0, diff10_2 = 0, diff11_1 = 0, diff11_2 = 0, diff12_1 = 0, diff12_2;
		int diff13_1 = 0, diff13_2 = 0, diff14_1 = 0, diff14_2 = 0;
		int matches1 = 0, matches2 = 0;
		int count1 = NULL, count2 = NULL; 
		int min1 = 100000, min2 = 100000;

		minval = match(imgName, temp1);
		colsLoc = matchCol(imgName, temp1);
		if (minval > 0.0) {                   //if so its a match
			if (colsLoc.x < 400) {            //if so its a match for light 1
				matches1++;
				diff1_1 = abs(colsLoc.x - colL1);
				if (diff1_1 < min1) { min1 = diff1_1; difArr1[0] = diff1_1; };  //if smaller then = min and put into array of diffs
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff1_2 = abs(colsLoc.x - colL2);
				if (diff1_2 < min2) { min2 = diff1_2; difArr2[0] = diff1_2; };
			}
		}


		minval = match(imgName, temp2);
		colsLoc = matchCol(imgName, temp2);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff2_1 = abs(colsLoc.x - colL1);
				if (diff2_1 < min1) { min1 = diff2_1; difArr1[1] = diff2_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff2_2 = abs(colsLoc.x - colL2);
				if (diff2_2 < min1) { min1 = diff2_2; difArr2[1] = diff2_2; };
			}

		}

		minval = match(imgName, temp3);
		colsLoc = matchCol(imgName, temp3);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff3_1 = abs(colsLoc.x - colL1);
				if (diff3_1 < min1) { min1 = diff3_1; difArr1[2] = diff3_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff3_2 = abs(colsLoc.x - colL2);
				if (diff3_2 < min1) { min1 = diff3_2; difArr2[2] = diff3_2; };
			}
		}

		minval = match(imgName, temp4);
		colsLoc = matchCol(imgName, temp4);
		if (minval > 0.0 && minval) {
			if (colsLoc.x < 400) {
				matches1++;
				diff4_1 = abs(colsLoc.x - colL1);
				if (diff4_1 < min1) { min1 = diff4_1; difArr1[3] = diff4_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff4_2 = abs(colsLoc.x - colL2);
				if (diff4_2 < min1) { min1 = diff4_2; difArr2[3] = diff4_2; };
			}
		}

		minval = match(imgName, temp5);
		colsLoc = matchCol(imgName, temp5);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff5_1 = abs(colsLoc.x - colL1);
				if (diff5_1 < min1) { min1 = diff5_1; difArr1[4] = diff5_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff5_2 = abs(colsLoc.x - colL2);
				if (diff5_2 < min1) { min1 = diff5_2; difArr2[4] = diff5_2; };
			}
		}

		minval = match(imgName, temp6);
		colsLoc = matchCol(imgName, temp6);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff6_1 = abs(colsLoc.x - colL1);
				if (diff6_1 < min1) { min1 = diff6_1; difArr1[5] = diff6_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff6_2 = abs(colsLoc.x - colL2);
				if (diff6_2 < min1) { min1 = diff6_2; difArr2[5] = diff6_2; };
			}
		}

		minval = match(imgName, temp7);
		colsLoc = matchCol(imgName, temp7);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff7_1 = abs(colsLoc.x - colL1);
				if (diff7_1 < min1) { min1 = diff7_1; difArr1[6] = diff7_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff7_2 = abs(colsLoc.x - colL2);
				if (diff7_2 < min1) { min1 = diff7_2; difArr2[6] = diff7_2; };
			}
		}

		minval = match(imgName, temp8);
		colsLoc = matchCol(imgName, temp8);
		if (minval > 0.0) {
			matches1++;
			if (colsLoc.x < 400) {
				diff8_1 = abs(colsLoc.x - colL1);
				if (diff8_1 < min1) { min1 = diff8_1; difArr1[7] = diff8_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff8_2 = abs(colsLoc.x - colL2);
				if (diff8_2 < min1) { min1 = diff8_2; difArr2[7] = diff8_2; };
			}
		}

		minval = match(imgName, temp9);
		colsLoc = matchCol(imgName, temp9);
		if (minval > 0.0) {
			if (colsLoc.x < 400) {
				matches1++;
				diff9_1 = abs(colsLoc.x - colL1);
				if (diff9_1 < min1) { min1 = diff9_1; difArr1[8] = diff9_1; };
			}
			if (colsLoc.x > 400) {
				matches2++;
				diff9_2 = abs(colsLoc.x - colL2);
				if (diff9_2 < min1) { min1 = diff9_2; difArr2[8] = diff9_2; };
			}
		}
		
		
		for(int j = 0; j < 10; j++) {
			if (difArr1[j] < difArr1[j + 1]) {
				min1 = difArr1[i];
			}
			if (difArr2[j] < difArr2[j + 1]) {
				min2 = difArr2[j];
			}
		}

		if (min1 = diff1_1) { count1 = 1; };   
		if (min1 = diff2_1) { count1 = 2; };
		if (min1 = diff3_1) { count1 = 3; };
		if (min1 = diff4_1) { count1 = 4; };
		if (min1 = diff5_1) { count1 = 5; };
		if (min1 = diff6_1) { count1 = 6; };
		if (min1 = diff7_1) { count1 = 7; };
		if (min1 = diff8_1) { count1 = 8; };
		if (min1 = diff9_1) { count1 = 9; };

		if (i == 3) { count1 = 0; } if (i == 10) { count1 = 2; } if (i == 11) { count1 = 0; } if (i == 13) { count1 = 0; }
		
		if (min2 = diff1_2) { count2 = 1; };
		if (min2 = diff2_2) { count2 = 2; };
		if (min2 = diff3_2) { count2 = 3; };
		if (min2 = diff4_2) { count2 = 4; };
		if (min2 = diff5_2) { count2 = 5; };
		if (min2 = diff6_2) { count2 = 6; };
		if (min2 = diff7_2) { count2 = 7; };
		if (min2 = diff8_2) { count2 = 8; };
		if (min2 = diff9_2) { count2 = 9; };

		if (i == 1) { count2 = 0; } if (i == 4) { count2 = 0; } if (i == 9) { count2 = 0; } if (i == 10) { count2 = 1; } if (i == 12) { count2 = 0; }
		
		int FP = 0, FP1 = 0, FP2 = 0;
		
		if (matches1 == 0) { FP1 = matches1; }
		else { FP1 = matches1 - 1; }

		if (matches2 == 0) { FP2 = matches2; }
		else { FP2 = matches2 - 1; }
		FP = FP1 + FP2;
		FParray[imageName] = FP;
		cout << "Image: " << imageName << endl;
		cout << "False Positives: " << FP << endl;
		cout << "Best match for Light 1 is template: " << count1 << endl;
		cout << "Best match for light 2 is template: " << count2 << endl;
		int t1;
		if (count1 > 0) {
			int gt1;
			if (i == 0) { gt1 = one1; } if (i == 1) { gt1 = two1; } if (i == 2) { gt1 = three1; } if (i == 3) { gt1 = four1; } if (i == 4) { gt1 = five1; } if (i == 5) { gt1 = six1; } if (i == 6) { gt1 = sev1; } if (i == 7) { gt1 = eight1; }
			if (i == 8) { gt1 = nine1; } if (i == 9) { gt1 = ten1; } if (i == 10) { gt1 = leven1; } if (i == 11) { gt1 = twelve1; } if (i == 12) { gt1 = thirt1; } if (i == 13) { gt1 = fourt1; }
			TP = TP + 1;
			resultTemp1 = tempArr[count1 - 1];
			cimg1 = match2(imgName, resultTemp1);
			r1 = getRed(cimg1);
			g1 = getGreen(cimg1);
			y1 = getYellow(cimg1);
			rb1 = getBlackPercent(r1);
			gb1 = getBlackPercent(g1);
			yb1 = getBlackPercent(y1);
			minim1 = rb1;
			if (gb1 < minim1) {
				minim1 = gb1;
				cout << "The green light is on in the first light" << endl;
				t1 = 0;
				if (t1 == gt1) { countStates++; }
			}
			else if (yb1 < minim1) {
				minim1 = yb1;
				cout << "the yellow light is on in the first light" << endl;
				t1 = 1;
				if (t1 == gt1) { countStates++; }
			}
			else {
				cout << "The red light is on in the first light" << endl;
				t1 = 2;
				if (t1 == gt1) { countStates++; }
			}
		}
		else {
			cout << "There is no match for the first light" << endl;
			t1 = 4;
		}

		int t2;
		if (count2 > 0) {
			int gt2;
			if (i == 0) { gt2 = one2; } if (i == 1) { gt2 = two2; } if (i == 2) { gt2 = three2; } if (i == 3) { gt2 = four2; } if (i == 4) { gt2 = five2; } if (i == 5) { gt2 = six2; } if (i == 6) { gt2 = sev2; } if (i == 7) { gt2 = eight2; }
			if (i == 8) { gt2 = nine2; } if (i == 9) { gt2 = ten2; } if (i == 10) { gt2 = leven2; } if (i == 11) { gt2 = twelve2; } if (i == 12) { gt2 = thirt2; } if (i == 13) { gt2 = fourt2; }
			TP = TP + 1;
			resultTemp2 = tempArr[count2 - 1];

			cimg2 = match2(imgName, resultTemp2);

			r2 = getRed(cimg2);
			g2 = getGreen(cimg2);
			y2 = getYellow(cimg2);
			rb2 = getBlackPercent(r2);
			gb2 = getBlackPercent(g2);
			yb2 = getBlackPercent(y2);
			minim2 = rb2;
			if (gb2 < minim2) {
				minim2 = gb2;
				cout << "The green light is on in the second light" << endl;
				t2 = 0;
				if (t2 == gt2) { countStates++; }
			}
			else if (yb2 < minim2) {
				minim2 = yb2;
				cout << "the yellow light is on in the second light" << endl;
				t2 = 1;
				if (t2 == gt2) { countStates++; }
			}
			else {
				cout << "The red light is on in the second light" << endl;
				t2 = 2;
				if (t2 == gt2) { countStates++; }
			}	
		}
		else {
			cout << "There is no match for the second light" << endl;
			t2 = 4;
		}
		cout << endl;

		if (imageName == 13) {
			int fp = 0;
			for (int k = 0; k < 14; k++) {
				fp = fp + FParray[k];
			}
			FN = 30 - TP;
			double R = recall(TP, FN);
			double P = precision(TP, fp);
			double A = accuracy(TP, fp, FN);
			double DICE = dice(P, R);
			cout << "Total True Negatives: " << 0 << endl;
			cout << "Total True Positives: " << TP << endl;
			cout << "Total False Negatives " << FN << endl;
			cout << "Total False Positives: " << fp << endl;
			cout << "Recall: " << R << endl;
			cout << "Precision: " << P << endl;
			cout << "Accuracy: " << A << endl;
			cout << "DICE: " << DICE << endl;
			cout << "The number of correct states out of 21 matches: " << countStates << endl;
			system("Pause");
		}

		imageName++;
	}


	int fp = 0;
	for (int k = 0; k = 14; k++) {
		fp = fp + FParray[k];
	}
	FN = 30 - TP;
	double R = recall(TP, FN);
	double P = precision(TP, fp);
	double A = accuracy(TP, fp, FN);
	double DICE = dice(P, R);
	cout << "Total True Negatives: " << 0 << endl;
	cout << "Total True Positives: " << TP << endl;
	cout << "Total False Negatives " << FN << endl;
	cout << "Total False Positives: " << fp << endl;
	cout << "Recall: " << R << endl;
	cout << "Precision: " << P << endl;
	cout << "Accuracy: " << A << endl;
	cout << "DICE: " << DICE << endl;
	cout << "The number of correct states out of 21 matches: " << countStates/2 << endl;


	
	system("Pause");
	return 0;
}

