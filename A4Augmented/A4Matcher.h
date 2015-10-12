#ifndef A4MATCHER_H
#define A4MATCHER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <array>

#include "LineClusterifier.h"
#include "PointClusterifier.h"
#include "BorderAnalyzer.h"
#include "LocalHoughTransformer.h"


class A4Matcher
{
	struct CornerRecord {
		CvPoint pt;
		int n;
	};

	int resizeFactor;
	CvSize mainSize;
	CvSize sizeII;
	CvSize sizeFactored;
	CvSize sizeFactoredII;
	IplImage *image, *imageResized;
	IplImage *imageCanny, *imageCannyResized;

	//Temporary memory banks needed for image processing
    IplImage *redChannel, *greenChannel, *blueChannel; //TODO: Theese may be optimized
    IplImage *redChannelResized, *greenChannelResized, *blueChannelResized;
    IplImage *redChannelResizedTmp, *greenChannelResizedTmp, *blueChannelResizedTmp;
    IplImage *redSobelH, *greenSobelH, *blueSobelH, *redSobelV, *greenSobelV, *blueSobelV;
	IplImage *horizontalBorders, *verticalBorders;
	
	IplImage *uBorders, *dBorders, *lBorders, *rBorders;
	IplImage *uBordersII, *dBordersII, *lBordersII, *rBordersII;
	
	IplImage *uBordersFactored, *dBordersFactored, *lBordersFactored, *rBordersFactored;
	IplImage *uBordersIIFactored, *dBordersIIFactored, *lBordersIIFactored, *rBordersIIFactored;

	IplImage *a4FoundMark;
	IplImage *buffer;
	
	LocalHoughTransformer lhtHor;
	LocalHoughTransformer lhtVer;
	
	void formatImage(IplImage *aimage);
	void channelSplit(IplImage *aimage);
	void applyColorInvalidator();
	void prepareDerivativesSearchTemplates();

	void applyURCornerSearch();
	void applyULCornerSearch();
	void applyDRCornerSearch();
	void applyDLCornerSearch();

	void applyBaseCornerSearch(std::list<CvPoint>& cornersList, uchar *dataHorBorder, uchar *dataVerBorder, int horStep, int verStep);
	
	void applyA4SearchMask();
	void findCorners();

	//bool applyWhiteBodyDetector(int directionX, int directionY, int x0, int y0);
	bool applyBorderDetector(uchar *dataBorder, int step, int orthogonalStep, int borderLookupSize, int maxFails);

	void addIntersectionsToCornersList();
	void normalizePoints();
	bool findA4();
	void clearResults();
	void clearMemory();

public:
	
	std::list<CvPoint> ULCorners;
	std::list<CvPoint> URCorners;
	std::list<CvPoint> DLCorners;
	std::list<CvPoint> DRCorners;
	std::list<CvPoint> testLines;
	
	struct A4PreDetectedRecord {
		CvPoint ulpt;
		CvPoint drpt;
		CvPoint ulptBorder;
		CvPoint drptBorder;
		A4PreDetectedRecord(CvPoint aulpt, CvPoint adrpt, CvPoint aulptBorder, CvPoint adrptBorder) : ulpt(aulpt), drpt(adrpt), ulptBorder(aulptBorder), drptBorder(adrptBorder) {}
	};
	std::list<A4PreDetectedRecord> A4PreDetected;

	struct A4Record {
		CvPoint UL;
		CvPoint UR;
		CvPoint DL;
		CvPoint DR;
		A4Record(CvPoint aUL, CvPoint aUR, CvPoint aDL, CvPoint aDR): UL(aUL), UR(aUR), DL(aDL), DR(aDR) {}
	};
	std::list<A4Record> foundA4;

	void analyseImage();
	void dump();
	void initMemory(CvSize size);
	A4Matcher();
	void setAndAnalyseImage(IplImage *aimage);
	~A4Matcher(void);
};


#endif