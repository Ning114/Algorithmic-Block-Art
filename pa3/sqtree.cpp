/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"
#include <cfloat>

 // First Node constructor, given.
SQtree::Node::Node(pair<int, int> ul, int w, int h, RGBAPixel a, double v)
	:upLeft(ul), width(w), height(h), avg(a), var(v), NW(NULL), NE(NULL), SE(NULL), SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats& s, pair<int, int> ul, int w, int h)
	: upLeft(ul), width(w), height(h), NW(NULL), NE(NULL), SE(NULL), SW(NULL) {
	avg = s.getAvg(ul, w, h);
	var = s.getVar(ul, w, h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
	clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree& other) {
	copy(other);
}

// SQtree assignment operator, given.
SQtree& SQtree::operator=(const SQtree& rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG& imIn, double tol) {
	stats stats(imIn);

	int width = imIn.width();
	int height = imIn.height();

	pair<int, int> topLeft(0, 0);

	root = buildTree(stats, topLeft, width, height, tol);

}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node* SQtree::buildTree(stats& s, pair<int, int>& ul, int w, int h, double tol) {

	//	cout << " ran function " << endl;
//	cout << "tol: " << tol << endl;

	//cout << s.getVar(ul, w, h) << endl;

	double minVar = DBL_MAX;

	int width = w;
	int height = h;

	// cout << "ul(" << ul.first << ", " << ul.second << ") width: " << w << "height: " << h << endl;
	//cout << "  " << endl;

	//wtf why variance is so high?
	double variance = s.getVar(ul, width, height);
	//cout << variance << endl;

	RGBAPixel average = s.getAvg(ul, w, h);


	if (variance <= tol || (w == 1 && h == 1)) {

		Node* node = new Node(ul, w, h, average, variance);
		node->NE = NULL;
		node->NW = NULL;
		node->SE = NULL;
		node->SW = NULL;

		return node;
	}

	pair<int, int> splitPoint(ul.first, ul.second);
	for (int i = ul.first; i < width + ul.first; i++) {
		for (int j = ul.second; j < height + ul.second; j++) {

			if (i == ul.first && j == ul.second) {
				
			} else {
				pair<int, int> topL(ul.first, ul.second);
			double topLVar = s.getVar(topL, i - ul.first, j - ul.second);
			//cout << "variance topLVar: " << topLVar << endl;

			pair<int, int> topR(i, ul.second);
			double topRVar = s.getVar(topR, width - (i - ul.first), j - ul.second);

			//cout << "variance topRVar: " << topRVar << endl;

			pair<int, int> botL(ul.first, j);
			double botLVar = s.getVar(botL, i - ul.first, height - (j - ul.second));


			//cout << "variance botLVar: " << botLVar << endl;

			pair<int, int> botR(i, j);
			double botRVar = s.getVar(botR, width - (i - ul.first), height - (j - ul.second));


			//cout << "variance botRVar: " << botRVar << endl;

			double varMax = max(max(topLVar, topRVar), max(botLVar, botRVar));

			if (varMax < minVar) {
				minVar = varMax;
				splitPoint.first = i;
				splitPoint.second = j;
			}
			}
		}
	}


	// cout << "SPLIT POINT: " << splitPoint.first << ", " << splitPoint.second << endl;

	Node* node = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));

	//if (splitPoint.first == ul.first && splitPoint.second == ul.second) {
		//cout << "-----NO SPLIT CASE-----" << endl;
		//return node;
	//}

	if (splitPoint.first == ul.first && splitPoint.second > ul.second) {
		// cout << "-----SPLIT HORIZONTALLY-----" << endl;
		// cout << "NE:" << endl;
		pair<int, int> topL(ul.first, ul.second);
		node->NE = buildTree(s, topL, w, splitPoint.second - ul.second, tol);

		// cout << "SE:" << endl;
		pair<int, int> botL(splitPoint.first, splitPoint.second);
		node->SE = buildTree(s, botL, w, h - (splitPoint.second - ul.second), tol);

		return node;
	}

	if (splitPoint.first > ul.first && splitPoint.second == ul.second) {
		// cout << "-----SPLIT VERTICALLY-----" << endl;
		// cout << "NW:" << endl;
		pair<int, int> topL(ul.first, ul.second);
		node->NW = buildTree(s, topL, splitPoint.first - ul.first, h, tol);

		// cout << "NE:" << endl;
		pair<int, int> topR(splitPoint.first, splitPoint.second);
		node->NE = buildTree(s, topR, w - (splitPoint.first - ul.first), h, tol);

		return node;
	}

	// cout << "-----4x SPLIT CASE-----" << endl;

	// cout << "NW:" << endl;
	pair<int, int> topL(ul.first, ul.second);
	node->NW = buildTree(s, topL, splitPoint.first - ul.first, splitPoint.second - ul.second, tol);

	// cout << "NE:" << endl;
	pair<int, int> topR(splitPoint.first, ul.second);
	node->NE = buildTree(s, topR, (w - (splitPoint.first - ul.first)), splitPoint.second - ul.second, tol);

	// cout << "SW:" << endl;
	pair<int, int> botL(ul.first, splitPoint.second);
	node->SW = buildTree(s, botL, splitPoint.first - ul.first, (h - (splitPoint.second - ul.second)), tol);

	// cout << "SE:" << endl;
	pair<int, int> botR(splitPoint.first, splitPoint.second);
	node->SE = buildTree(s, botR, (w - (splitPoint.first - ul.first)), (h - (splitPoint.second - ul.second)), tol);

	//cout << "  " << endl;

	return node;

}

/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {

	// cout << "called render()" << endl;
	PNG picture = PNG(root->width, root->height);

	// cout << "width: ";
	// 			cout << root->width << endl;
	// 			cout << "height: ";
	// 			cout << root->height << endl;

	// cout << "created picture" << endl;
	renderHelper(root, picture);

	return picture;
}

void SQtree::renderHelper(Node* node, PNG& pic) {
	//cout << &*node << endl;

	//cout << "in render helper" << endl;
	if (node == NULL) {
		//cout << "base " << endl;
		return;
	}

	pair<int, int> ul = node->upLeft;
	int width = node->width;
	int height = node->height;


	if ((node->NE == NULL) && (node->NW == NULL) && (node->SE == NULL) && (node->SW == NULL)) {

		for (int i = ul.first; i < ul.first + width; i++) {
			for (int j = ul.second; j < ul.second + height; j++) {

				// cout << "ul.second: ";
				// cout << ul.second << endl;
				// cout << "height: ";
				// cout << height << endl;
				

				// cout << "i: ";
				// cout << i << endl;
				// cout << "j: ";
				// cout << j << endl;

				RGBAPixel* currPixel = pic.getPixel(i, j);
				*currPixel = node->avg;

			}
		}
		
	} else {
	renderHelper(node->NE, pic);

	renderHelper(node->NW, pic);

	renderHelper(node->SE, pic);

	renderHelper(node->SW, pic);
	}

	


}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
	deleteNode(root);
}

void SQtree::copy(const SQtree& other) {
	cout << " before copy " << endl;
	root = copyNode(other.root);
	cout << " after copy " << endl;
}

int SQtree::size() {
	return treeSize(root);
}

void SQtree::deleteNode(Node* node) {
	if (node == NULL) return;

	deleteNode(node->NE);
	deleteNode(node->NW);
	deleteNode(node->SE);
	deleteNode(node->SW);

	free(node);
	node = NULL;

}

SQtree::Node* SQtree::copyNode(Node* other) {
	if (other == NULL) return NULL;
	Node* temp = new Node(other->upLeft, other->width, other->height, other->avg, other->var);

	temp->NE = copyNode(other->NE);

	temp->NW = copyNode(other->NW);

	temp->SE = copyNode(other->SE);

	temp->SW = copyNode(other->SW);

	return temp;
}

int SQtree::treeSize(Node* node) {
	if (node == NULL) {
		return 0;
	} else {
		return 1 + treeSize(node->NE) + treeSize(node->NW) + treeSize(node->SE) + treeSize(node->SW);
	}
}




