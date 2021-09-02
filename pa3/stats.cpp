#include "stats.h"


#include "stats.h"

stats::stats(PNG& im) {
	int height = im.height();
	int width = im.width();

	//clean vecotrs just in case
	sumRed.clear();
	sumGreen.clear();
	sumBlue.clear();
	sumsqRed.clear();
	sumsqGreen.clear();
	sumsqBlue.clear();

	//resize to image size
	sumRed.resize(width, vector<long>(height));
	sumGreen.resize(width, vector<long>(height));
	sumBlue.resize(width, vector<long>(height));
	sumsqRed.resize(width, vector<long>(height));
	sumsqGreen.resize(width, vector<long>(height));
	sumsqBlue.resize(width, vector<long>(height));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			RGBAPixel* pixel = im.getPixel(i, j);

			double rSum = pixel->r;
			double gSum = pixel->g;
			double bSum = pixel->b;

			double rsqSum = (pixel->r) * (pixel->r);
			double gsqSum = (pixel->g) * (pixel->g);
			double bsqSum = (pixel->b) * (pixel->b);


			if (i > 0 && j > 0) {
				rSum += sumRed[i - 1][j] + sumRed[i][j - 1] - sumRed[i - 1][j - 1];
				gSum += sumGreen[i - 1][j] + sumGreen[i][j - 1] - sumGreen[i - 1][j - 1];
				bSum += sumBlue[i - 1][j] + sumBlue[i][j - 1] - sumBlue[i - 1][j - 1];

				//sq
				rsqSum += sumsqRed[i - 1][j] + sumsqRed[i][j - 1] - sumsqRed[i - 1][j - 1];
				gsqSum += sumsqGreen[i - 1][j] + sumsqGreen[i][j - 1] - sumsqGreen[i - 1][j - 1];
				bsqSum += sumsqBlue[i - 1][j] + sumsqBlue[i][j - 1] - sumsqBlue[i - 1][j - 1];

			}

			if (i == 0 && j > 0) {
				rSum += sumRed[i][j - 1];
				gSum += sumGreen[i][j - 1];
				bSum += sumBlue[i][j - 1];

				//sq
				rsqSum += sumsqRed[i][j - 1];
				gsqSum += sumsqGreen[i][j - 1];
				bsqSum += sumsqBlue[i][j - 1];


			}
			if (i > 0 && j == 0) {
				rSum += sumRed[i - 1][j];
				gSum += sumGreen[i - 1][j];
				bSum += sumBlue[i - 1][j];

				//sq
				rsqSum += sumsqRed[i - 1][j];
				gsqSum += sumsqGreen[i - 1][j];
				bsqSum += sumsqBlue[i - 1][j];
			}
			sumRed[i][j] = rSum;
			sumGreen[i][j] = gSum;
			sumBlue[i][j] = bSum;
			sumsqRed[i][j] = rsqSum;
			sumsqGreen[i][j] = gsqSum;
			sumsqBlue[i][j] = bsqSum;

		}
	}
}


long stats::getSum(char channel, pair<int, int> ul, int w, int h) {
	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			return sumRed[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumRed[ul.first + w - 1][ul.second + h - 1]
				- sumRed[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumRed[ul.first + w - 1][ul.second + h - 1]
				- sumRed[ul.first - 1][ul.second + h - 1];
		}
		return sumRed[ul.first + w - 1][ul.second + h - 1]
			+ sumRed[ul.first - 1][ul.second - 1]
			- sumRed[ul.first + w - 1][ul.second - 1]
			- sumRed[ul.first - 1][ul.second + h - 1];
	}
	else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			return sumGreen[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumGreen[ul.first + w - 1][ul.second + h - 1]
				- sumGreen[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumGreen[ul.first + w - 1][ul.second + h - 1]
				- sumGreen[ul.first - 1][ul.second + h - 1];
		}
		return sumGreen[ul.first + w - 1][ul.second + h - 1]
			+ sumGreen[ul.first - 1][ul.second - 1]
			- sumGreen[ul.first + w - 1][ul.second - 1]
			- sumGreen[ul.first - 1][ul.second + h - 1];
	}
	else {
		if (ul.first == 0 && ul.second == 0) {
			return sumBlue[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumBlue[ul.first + w - 1][ul.second + h - 1]
				- sumBlue[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumBlue[ul.first + w - 1][ul.second + h - 1]
				- sumBlue[ul.first - 1][ul.second + h - 1];
		}
		return sumBlue[ul.first + w - 1][ul.second + h - 1]
			+ sumBlue[ul.first - 1][ul.second - 1]
			- sumBlue[ul.first + w - 1][ul.second - 1]
			- sumBlue[ul.first - 1][ul.second + h - 1];
	}
}

long stats::getSumSq(char channel, pair<int, int> ul, int w, int h) {
	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqRed[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumsqRed[ul.first + w - 1][ul.second + h - 1]
				- sumsqRed[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumsqRed[ul.first + w - 1][ul.second + h - 1]
				- sumsqRed[ul.first - 1][ul.second + h - 1];
		}
		return sumsqRed[ul.first + w - 1][ul.second + h - 1]
			+ sumsqRed[ul.first - 1][ul.second - 1]
			- sumsqRed[ul.first + w - 1][ul.second - 1]
			- sumsqRed[ul.first - 1][ul.second + h - 1];
	}
	else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqGreen[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumsqGreen[ul.first + w - 1][ul.second + h - 1]
				- sumsqGreen[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumsqGreen[ul.first + w - 1][ul.second + h - 1]
				- sumsqGreen[ul.first - 1][ul.second + h - 1];
		}
		return sumsqGreen[ul.first + w - 1][ul.second + h - 1]
			+ sumsqGreen[ul.first - 1][ul.second - 1]
			- sumsqGreen[ul.first + w - 1][ul.second - 1]
			- sumsqGreen[ul.first - 1][ul.second + h - 1];
	}
	else {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqBlue[w - 1][h - 1];
		}
		if (ul.first == 0 && ul.second > 0) {
			return sumsqBlue[ul.first + w - 1][ul.second + h - 1]
				- sumsqBlue[ul.first + w - 1][ul.second - 1];
		}
		if (ul.first > 0 && ul.second == 0) {
			return sumsqBlue[ul.first + w - 1][ul.second + h - 1]
				- sumsqBlue[ul.first - 1][ul.second + h - 1];
		}
		return sumsqBlue[ul.first + w - 1][ul.second + h - 1]
			+ sumsqBlue[ul.first - 1][ul.second - 1]
			- sumsqBlue[ul.first + w - 1][ul.second - 1]
			- sumsqBlue[ul.first - 1][ul.second + h - 1];
	}
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
long stats::getVar(pair<int, int> ul, int w, int h) {
	int numberOfPixels = w * h;
	if (numberOfPixels != 0) {
		double redVar = getSumSq('r', ul, w, h) - ((getSum('r', ul, w, h)) * (getSum('r', ul, w, h)) / numberOfPixels);
		double greenVar = getSumSq('g', ul, w, h) - ((getSum('g', ul, w, h)) * (getSum('g', ul, w, h)) / numberOfPixels);
		double blueVar = getSumSq('b', ul, w, h) - ((getSum('b', ul, w, h)) * (getSum('b', ul, w, h)) / numberOfPixels);
		double variability = redVar + greenVar + blueVar;
		return variability;
	}
	return 0;

}

RGBAPixel stats::getAvg(pair<int, int> ul, int w, int h) {

	double redSum = getSum('r', ul, w, h);
	double greenSum = getSum('g', ul, w, h);
	double blueSum = getSum('b', ul, w, h);
	int numberOfPixels = w * h;
	RGBAPixel pixel = RGBAPixel(redSum / numberOfPixels, greenSum / numberOfPixels, blueSum / numberOfPixels);
	return pixel;
}

vector< vector< long >> stats::getRed() {
	return sumRed;
}
vector< vector< long >> stats::getRedsq() {
	return sumsqRed;
}
vector< vector< long >> stats::getGreen() {
	return sumGreen;
}
vector< vector< long >> stats::getGreensq() {
	return sumsqGreen;
}
vector< vector< long >> stats::getBlue() {
	return sumBlue;
}
vector< vector< long >> stats::getBluesq() {
	return sumsqBlue;
}