#include <iomanip>
#include <iostream>
#include "boost/format.hpp"
#include <fstream>
#include <bitset>
#include <sstream>
#include <chrono>
#include <ctime>
#include "unistd.h"
#include "piranha.h"
#include "playback.h"
#include "driver.h"
#include "setup.h"

#define IMAGE_FAST		1
#define IMAGE_FPN 		2
#define IMAGE_SLOW 		3

#define TRACK_REGULAR 	1
#define TRACK_EMBED 	2
#define TRACK_TRAJL		3
#define TRACK_TRAJNL	4
#define TRACK_RPM		5

using namespace std;
using namespace cv;

UIsetup *uiSetup;
UIplayback *playback;
Driver camera;
string serial = "NUMATOV1BB";
// string serial = "UM232H";

/*
 *  ----------------------------------------------------------------------------------------------
 *                                     Program Support Matrix
 *  ----------------------------------------------------------------------------------------------
 *						Regular			CDS			Parallel		Threshold 
 *              ----------------------------------------------------------------------
 *  Image				X				X			X				
 *  Image FPN							X			X				
 *  Image Long							X			X				
 *  RCR					X							X				
 *  Edge				X							X				X
 *  Median				X				X			X				
 *  DCT					X							X				
 *  Histogram			X				X			X				
 *  Tracking			X				X			X				X
 *  Tracking Embed		X				X			X				X
 *  Track Trajectory	X				X			X				X
 *  Tracking RPM						X			X				X
 *  AER									X			X				
 *
 *  ----------------------------------------------------------------------------------------------
 *
 */

         int idxProgram   = 0,
             idxImage     = 0,
             idxRCR       = 1,
             idxEdge      = 2,
             idxMedian    = 3,
             idxDCT       = 4,
             idxHistogram = 5,
             idxTracking  = 6,
             idxAER       = 7,
             idxBench     = 8;

static const string programdir = "programs";
static const string programNames[] = {
		                        "imaging",
		                        "rcr",
		                        "edge",
		                        "median",
		                        "dct",
		                        "histogram",
		                        "tracking",
		                        "aer",
		                        "benchmark"
     				       	};

 static const string asmImage = "img-fast.txt",
                  asmCDSImage = "img-fast-cds.txt",
                 asmSlowImage = "img-slow.txt",
              asmCDSSlowImage = "img-slow-cds.txt",
                  asmFPNImage = "img-fpn.txt",
       
                       asmRCR = "rcr_readout.txt",
       
                      asmEdge = "edge_all.txt",
                      
                    asmMedian = "median3.txt",
                 asmCDSMedian = "median3-cds.txt",
                      
                       asmDCT = "dct.txt",
              
                 asmHistogram = "histogram.txt",
              asmCDSHistogram = "histogram-cds.txt",
                     
                     asmTrack = "tracking.txt",
                  asmCDSTrack = "tracking-cds.txt",
                asmEmbedTrack = "tracking-embed.txt",
             asmCDSEmbedTrack = "tracking-embed-cds.txt",
           asmLinearTrajTrack = "trajectory-linear.txt",
        asmCDSLinearTrajTrack = "trajectory-linear-cds.txt",
        asmNonlinearTrajTrack = "trajectory-nonlinear.txt",
     asmCDSNonlinearTrajTrack = "trajectory-nonlinear-cds.txt",
                       asmRPM = "rpm.txt",
       
                       asmAER = "aer.txt",
              
                     asmBench = "bench.txt";
  
       unsigned int bpImage[] = {8},
                 bpCDSImage[] = {0, 13, 28},
                 bpFPNImage[] = {0, 13, 28},
                bpSlowImage[] = {19},
             bpCDSSlowImage[] = {0, 13, 47},
                      
                      bpRCR[] = {12},
                     
                     bpEdge[] = {117},
                 threshEdge[] = {43, 55, 87, 99},
       
                   bpMedian[] = {350, 23},
                bpCDSMedian[] = {0, 12, 362},
                
                      bpDCT[] = {379},
                
                bpHistogram[] = {280},
             bpCDSHistogram[] = {0, 10, 280},
                    
                    bpTrack[] = {242},
                threshTrack[] = {59},
                 bpCDSTrack[] = {8, 20, 268},
             threshCDSTrack[] = {85},
               bpEmbedTrack[] = {258},
           threshEmbedTrack[] = {77},
            bpCDSEmbedTrack[] = {8, 20, 287},
        threshCDSEmbedTrack[] = {83},

          bpLinearTrajTrack[] = {330},
      threshLinearTrajTrack[] = {57},
       bpCDSLinearTrajTrack[] = {0, 20, 374},
   threshCDSLinearTrajTrack[] = {93},
       bpNonlinearTrajTrack[] = {330},
   threshNonlinearTrajTrack[] = {57},
    bpCDSNonlinearTrajTrack[] = {9, 10, 487},
threshCDSNonlinearTrajTrack[] = {68},

                   bpCDSRPM[] = {8, 20, 290},
                  threshRPM[] = {82},
       
                      bpAER[] = {0},
       
                    bpBench[] = {19};

vector<unsigned int> threshLinesEdge (threshEdge, threshEdge + sizeof(threshEdge) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesTrack (threshTrack, threshTrack + sizeof(threshTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesCDSTrack (threshCDSTrack, threshCDSTrack + sizeof(threshCDSTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesEmbedTrack (threshEmbedTrack, threshEmbedTrack + sizeof(threshEmbedTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesLinearTrajTrack (threshLinearTrajTrack, threshLinearTrajTrack + sizeof(threshLinearTrajTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesCDSLinearTrajTrack (threshCDSLinearTrajTrack, threshCDSLinearTrajTrack + sizeof(threshCDSLinearTrajTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesNonlinearTrajTrack (threshNonlinearTrajTrack, threshNonlinearTrajTrack + sizeof(threshNonlinearTrajTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesCDSNonlinearTrajTrack (threshCDSNonlinearTrajTrack, threshCDSNonlinearTrajTrack + sizeof(threshCDSNonlinearTrajTrack) / sizeof(unsigned int)); 
vector<unsigned int> threshLinesRPM (threshRPM, threshRPM + sizeof(threshRPM) / sizeof(unsigned int)); 

vector<int> paramspxm, paramspng;

string uistr = "UI: ";
string windowName = "PIRANHA";

bool debug = false, dummy = false, stretch = true, flipimg = true, transformimg = true, recording = false, cds = true, parallel = false, repeatRecord = true;
unsigned int imageMode = 0, trackMode = 0;
unsigned int stretchBy = 10;

Mat histLSB    = Mat(Size(80, 64), CV_8UC1, Scalar(0));
Mat histMSB    = Mat(Size(80, 64), CV_8UC1, Scalar(0));
Mat trackData  = Mat(Size(80, 64), CV_8UC1, Scalar(0));
Mat imgGray    = Mat(Size(80, 64), CV_8UC1, Scalar(0));
Mat img        = Mat(Size(80, 64), CV_8UC3, Scalar(0,0,0));
Mat imgStretch = Mat(Size(80 * stretchBy, 64 * stretchBy), CV_8UC3, Scalar(0,0,0));

//Scalar colorLine   = Scalar(144, 0, 144); // purple, hard to see
//Scalar colorPredict= Scalar(255, 0,   0); // red, actually shows up really hard to see blue in recording

Scalar colorTrack    = Scalar(0, 255, 255); // cyan, records as yellow because RGB vs BGR
Scalar colorLine     = Scalar(255, 0, 255); // magenta
Scalar colorPredict  = Scalar(255,191,  0); // records as light blue

vector<dataCoor> coorList, predictList;
unsigned int coorIdx = 0, coorMax = 24, predictMax = 3;
unsigned int coorx, coory, coorxn, cooryn, coorxold = 0, cooryold = 0, coorxnold = 0, coorynold = 0;
const unsigned int predictMaxL = 3, predictMaxNL = 10;
const unsigned int predictTable[predictMaxNL] = {
		 86,  // xp0 = x[14]
		160,  // xp1 = x[16]
		162,  // xp2 = x[18]
		164,  // xp3 = x[20]
		166,  // xp4 = x[22]
		240,  // xp5 = x[24]
		242,  // xp6 = x[26]
		244,  // xp7 = x[28]
		246,  // xp8 = x[30]
		320   // xp9 = x[32]
	};

VideoWriter videoOut;
unsigned int NUM_ROWS = 8, NUM_COLS = 10;

chrono::high_resolution_clock::time_point timenow, timeold, timeimg;
chrono::duration<double> timespan, timeproc;
vector<uint> bytesinit, bytesprogram, bytestotal, bytesimage, bytesdata, bytesrepeat;

double fps = 0, fpsmin = 0, fpsmax = 0, fpsavg = 0, fpsavgcount = 25, fpscount = 0, fpstotal = 0;
double clockns = 50; // 50 ns clock
double        trst_val, tint_val, vmax_val, vmin_val, vrst_val, vbias_val, timeout_val;
unsigned long trst,     tint,     vmax,     vmin,     vrst,     vbias;
unsigned long frameCount = 0, frameMax = 0, frameSuccess = 0;
bool decodeBP = false, decodeBPUpper = false, decodeCDSBP = false, decodeCDSBPUpper = false, decodeWrite = false, decodeCount = false, decodeCountUpper = false, encoderTranscode = false, encoderStretchable = false;
int encoderCodec = CV_FOURCC('H','F','Y','U'), encoderFPS = 30;
const int barIncrements = 10, errorMax = 100, predictIntervalDefault = 100;
      int barUpdate,          errorCount = 0, predictInterval = 100, predictUpdate = 0;

stringstream renamer;
QString lastDir = "";

UIpiranha::UIpiranha()
{
	setupUI(this);
	
	connect(actionRecord, SIGNAL(triggered()), this, SLOT(record_start()));
	connect(actionExport, SIGNAL(triggered()), this, SLOT(export_image()));
	connect(actionSetup,  SIGNAL(triggered()), this, SLOT(setup()));
	connect(actionExit,   SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(comboProgram, SIGNAL(currentIndexChanged(int)), this, SLOT(programChanged(int)));

	connect(btnApply,       SIGNAL(released()), this, SLOT(apply()));
	connect(btnStart,       SIGNAL(released()), this, SLOT(start()));
	connect(btnStop,        SIGNAL(released()), this, SLOT(stop()));
	connect(btnReset,       SIGNAL(released()), this, SLOT(reset()));
	connect(btnExportImage, SIGNAL(released()), this, SLOT(export_image()));
	connect(btnRecordStart, SIGNAL(released()), this, SLOT(record_start()));
	connect(btnRecordStop,  SIGNAL(released()), this, SLOT(record_stop()));

	connect(grpImage, SIGNAL(buttonReleased(int)), this, SLOT(imageChanged(int)));
	connect(grpTrack, SIGNAL(buttonReleased(int)), this, SLOT(trackChanged(int)));
	connect(grpFlags, SIGNAL(buttonReleased(int)), this, SLOT(flagsChanged(int)));
	
	initValues();
	reset();
	programChanged(0);

	rdoImageSlow->setChecked(true);
	imageChanged(grpImage->id(rdoImageSlow));
	rdoTrackEmbed->setChecked(true);
	trackChanged(grpTrack->id(rdoTrackEmbed));
	tabConfig->setCurrentIndex(0);
	comboProgram->setFocus();

	bytesinit.reserve(33);
	bytesprogram.reserve(1024);
	bytestotal.reserve(1536);
	bytesimage.reserve(10);
	bytesdata.reserve(10);

	paramspxm.push_back(CV_IMWRITE_PXM_BINARY);
	paramspxm.push_back(0);
	paramspng.push_back(CV_IMWRITE_PNG_COMPRESSION);
	paramspng.push_back(0);
	
	lblImage->setPixmap(QPixmap::fromImage(mat2qimage(imgStretch)));

	if (debug)
		chkDebug->setChecked(true);

	if (debug) 
		cout << uistr << "Initialized" << endl;
}

UIpiranha::~UIpiranha()
{
	if (debug) 
		cout << uistr << "Closed Connection" << endl;
	camera.disconnect();
	if (uiSetup)
		uiSetup->close();
}

void UIpiranha::initValues()
{
	if (debug) cout << uistr << "Initialize Values" << endl;
	spinVmax->setValue(2.5);
	spinVmin->setValue(0);
	spinVrst->setValue(2.20);
	spinVbias->setValue(0.30);
	spinTrst->setValue(5);
	spinTint->setValue(1000);
	spinTimeout->setValue(5);
	spinFrames->setValue(300);
	spinThreshold->setValue(64);
	spinDataPoints->setValue(coorMax);
	spinPredictPoints->setValue(predictMax);
	spinPredictInterval->setValue(predictIntervalDefault);

	// set checkboxes
	chkCDS->setChecked(cds);
	chkParallel->setChecked(parallel);
	chkRepeat->setChecked(repeatRecord);
	chkDebug->setChecked(debug);
	chkDummy->setChecked(dummy);
	chkStretch->setChecked(stretch);
	chkFlip->setChecked(flipimg);
	chkTransform->setChecked(transformimg);
	
	// hook any actions they affect
	flagsChanged(grpFlags->id(chkCDS));
	flagsChanged(grpFlags->id(chkParallel));
	flagsChanged(grpFlags->id(chkRepeat));
	flagsChanged(grpFlags->id(chkDebug));
	flagsChanged(grpFlags->id(chkDummy));
	flagsChanged(grpFlags->id(chkStretch));
	flagsChanged(grpFlags->id(chkFlip));
	flagsChanged(grpFlags->id(chkTransform));
	
	btnStart->setEnabled(false);
	btnStop->setEnabled(false);
	btnRecordStop->setEnabled(false);
}
	

void UIpiranha::programChanged(int programIndex)
{
	if (programIndex == idxImage)
	{
		layoutOpts->setCurrentIndex(1);
	}
	else if (programIndex == idxTracking)
	{
		layoutOpts->setCurrentIndex(2);
	}
	else
	{
		layoutOpts->setCurrentIndex(0);

		     if (programIndex == idxRCR)
			
			lblDesc->setText("Fixed pattern to verify readout settings. Best viewed with Transform disabled");

		else if (programIndex == idxEdge)
			
			lblDesc->setText("Horizontal and Vertical Edge Detection");

		else if (programIndex == idxMedian)
			
			lblDesc->setText("Median 3 compressed image");

		else if (programIndex == idxDCT)
			
			lblDesc->setText("Discrete cosine transform in 8x8 blocks");

		else if (programIndex == idxHistogram)
			
			lblDesc->setText("Requests image and two data packets (LSB/MSB)");

		else if (programIndex == idxAER)
			
			lblDesc->setText("Use Address-Event Representation. Requests Keyframe, then 100 Frames");

		else if (programIndex == idxBench)
			
			lblDesc->setText("Benchmark the FPGA RAM to USB communications. No PIRANHA activity");
	}

	if (programIndex == idxRCR || programIndex == idxEdge || programIndex == idxDCT || programIndex == idxBench)
	{
		chkCDS->setChecked(false);
		chkCDS->setEnabled(false);
	}
	else if ((programIndex == idxImage && imageMode == IMAGE_FPN) || programIndex == idxAER)
	{
		chkCDS->setChecked(true);
		chkCDS->setEnabled(false);
	}
	else
	{
		chkCDS->setEnabled(true);
	}

	(chkCDS->checkState() == Qt::Checked) ? cds = true : cds = false;

	(programIndex == idxEdge || programIndex == idxTracking) ?  lblThreshold->setVisible(true) :  lblThreshold->setVisible(false);
	(programIndex == idxEdge || programIndex == idxTracking) ? spinThreshold->setVisible(true) : spinThreshold->setVisible(false);

}

void UIpiranha::imageChanged(int imageFlagIndex)
{
	QRadioButton* tmpRadio = (QRadioButton*) grpImage->button(imageFlagIndex);
	if (tmpRadio)
	{
		if (tmpRadio == rdoImage) {
			imageMode = IMAGE_FAST;
			chkCDS->setEnabled(true);
		}
		else if (tmpRadio == rdoImageSlow) {
			chkCDS->setEnabled(true);
			imageMode = IMAGE_SLOW;
		}
		else if (tmpRadio == rdoImageFPN) {
			chkCDS->setChecked(true);
			chkCDS->setEnabled(false);
			imageMode = IMAGE_FPN;
		}

	    cout << uistr << "Image Mode: " << tmpRadio->text().toStdString() << endl;
	}
}

void UIpiranha::trackChanged(int trackFlagIndex)
{
	QRadioButton* tmpRadio = (QRadioButton*) grpTrack->button(trackFlagIndex);
	if (tmpRadio)
	{
		if (tmpRadio == rdoTrack) {
			chkCDS->setEnabled(true);
			trackMode = TRACK_REGULAR;
		}
		else if (tmpRadio == rdoTrackEmbed) {
			chkCDS->setEnabled(true);
			trackMode = TRACK_EMBED;
		}
		else if (tmpRadio == rdoTrackTrajLinear) {
			chkCDS->setEnabled(true);
			trackMode = TRACK_TRAJL;
			spinPredictPoints->setMaximum(predictMaxL);
		}
		else if (tmpRadio == rdoTrackTrajNonlinear) {
			chkCDS->setEnabled(true);
			trackMode = TRACK_TRAJNL;
			spinPredictPoints->setMaximum(predictMaxNL);
		}
		else if (tmpRadio == rdoTrackRPM) {
			trackMode = TRACK_RPM;
			chkCDS->setChecked(true);
			chkCDS->setEnabled(false);
		}

	    cout << uistr << "Track Mode: " << tmpRadio->text().toStdString() << endl;

		(trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL) ?      lblPredictPoints->setVisible(true) :      lblPredictPoints->setVisible(false);
		(trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL) ?     spinPredictPoints->setVisible(true) :     spinPredictPoints->setVisible(false);
		(trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL) ? lblPredictPointsUnits->setVisible(true) : lblPredictPointsUnits->setVisible(false);

	}
}

void UIpiranha::flagsChanged(int flagIndex)
{
	QCheckBox* chkFlag = (QCheckBox*) grpFlags->button(flagIndex);
	if (chkFlag)
	{
		bool state = chkFlag->isChecked();

		if (chkFlag == chkCDS) {
			
			cds = state;
			
			if (cds)
			{
				rdoImageFPN->setEnabled(true);
				rdoImageSlow->setEnabled(true);
				rdoTrackRPM->setEnabled(true);
			}
			else
			{
				if (rdoImageFPN->isChecked())
				{
					rdoImageFPN->setChecked(false);
					rdoImage->setChecked(true);
				}

				rdoImageSlow->setEnabled(true);
				rdoImageFPN->setEnabled(false);
				rdoTrackRPM->setEnabled(false);
			};
		}
		
		else if (chkFlag == chkParallel)
			
			parallel = state;
		
		else if (chkFlag == chkRepeat) {
			
			repeatRecord = state;
			
			if (repeatRecord) {
				
				parallel = true;
				chkParallel->setChecked(true);

			}
			
			chkParallel->setEnabled(!repeatRecord);

		}
		else if (chkFlag == chkDebug)
			
			debug = state;
		
		else if (chkFlag == chkDummy)
			
			dummy = state;
		
		else if (chkFlag == chkStretch)
			
			stretch = state;
		
		else if (chkFlag == chkFlip)
			
			flipimg = state;
		
		else if (chkFlag == chkTransform)
			
			transformimg = state;
		
		
		cout << uistr << "Flags Changed: " << setw(20) << left << chkFlag->text().toStdString() << ((state) ? "Enabled" : "Disabled") << endl;
		
		if (debug)
		{
			cout << "\t" << setw(12) << left << "CDS: " 		<< boolalpha << cds << endl;
			cout << "\t" << setw(12) << left << "Parallel: " 	<< boolalpha << parallel << endl;
			cout << "\t" << setw(12) << left << "Repeat: " 		<< boolalpha << repeatRecord << endl;
			cout << "\t" << setw(12) << left << "Debug: " 		<< boolalpha << debug << endl;
			cout << "\t" << setw(12) << left << "Dummy: " 		<< boolalpha << dummy << endl;
			cout << "\t" << setw(12) << left << "Stretch: " 	<< boolalpha << stretch << endl;
			cout << "\t" << setw(12) << left << "Flip: " 	    << boolalpha << flipimg << endl;
			cout << "\t" << setw(12) << left << "Transform: " 	<< boolalpha << transformimg << endl;
		}
	}
}

void UIpiranha::setup()
{
	uiSetup = new UIsetup();
	uiSetup->Device(&serial);
	uiSetup->Debug(debug);
	uiSetup->show();
}

// menu options: file -> open, save etc
// maybe implement saved states later?

//void UIpiranha::open()
//{
//	if (debug) cout << uistr << "Open" << endl//
//
//	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("PIRANHA files (*.pir)"))//
//
//	if (filename != "") {
//		QFile file(filename)//
//		if (!file.open(QIODevice::ReadOnly)) {
//			QMessageBox::critical(this, tr("Error"), tr("Could not open file"))//
//		}
//		else {
//			// more code to parse file
//			cout << uistr << "Opened " << filename.toStdString() << endl//
//			file.close()//
//		}
//	}
//}
//
//void UIpiranha::save()
//{
//	if (debug) cout << uistr << "Save" << endl//
//
//	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("PIRANHA files (*.pir)"))//
//
//	if (filename != "") {
//		QFile file(filename)//
//		if (!file.open(QIODevice::WriteOnly)) {
//			QMessageBox::critical(this, tr("Error"), tr("Could not save file"))//
//		}
//		else {
//			cout << uistr << "Saved " << filename.toStdString() << endl//
//			file.close()//
//		}
//	}
//}

void UIpiranha::exit()
{
	if (debug) cout << uistr << "Exit" << endl;
	close();
}

void UIpiranha::apply()
{
	if (debug) cout << uistr << "Apply" << endl;

	camera.disconnect();

	errorCount = 0;

	idxProgram = comboProgram->currentIndex();

	bytesinit.clear();
	bytesprogram.clear();
	bytestotal.clear();
	bytesimage.clear();
	bytesdata.clear();
	bytesrepeat.clear();

	if (idxProgram == idxEdge || idxProgram == idxTracking)
	{
		coorMax		= spinDataPoints->value();
		predictMax	= spinPredictPoints->value();
		predictInterval = spinPredictInterval->value();
		coorList.assign(coorMax, dataCoor(0,0));
		predictList.assign(predictMax, dataCoor(0,0));
		coorxold = 0; cooryold = 0;
		
		if (debug) 
			
			cout << uistr << "Data Pts:\t" << coorMax << endl;
		
		if (debug && (trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL)) 
			
			cout << uistr << boost::format("Predicting %d points every %d frames\t") % predictMax % predictInterval << endl;
	}

	bytesinit.push_back(0x03); // stop
	bytesinit.push_back(0x0a); // reset

	trst_val = spinTrst->value();
	trst = trst_val * 1000 / clockns;
	
	if (debug) cout << uistr << "Trst:\t" << trst_val << "\t| " << trst << endl;

	bytesinit.push_back(0x0b); // trst
	bytesinit.push_back((trst / 0x000000001) % 256);
	bytesinit.push_back((trst / 0x000000100) % 256);
	bytesinit.push_back((trst / 0x000010000) % 256);
	bytesinit.push_back((trst / 0x001000000) % 256);
	bytesinit.push_back((trst / 0x100000000) % 256);
	
	tint_val = spinTint->value();
	tint = (tint_val / clockns) * 1000;
	
	if (debug) cout << uistr << "Tint:\t" << tint_val << "\t| " << tint << endl;

	bytesinit.push_back(0x0c); // tint
	bytesinit.push_back((tint / 0x000000001) % 256);
	bytesinit.push_back((tint / 0x000000100) % 256);
	bytesinit.push_back((tint / 0x000010000) % 256);
	bytesinit.push_back((tint / 0x001000000) % 256);
	bytesinit.push_back((tint / 0x100000000) % 256);
	
	vmax_val = spinVmax->value();
	vmax = vmax_val / 2.5 * (pow(2,16) - 1);
	if (debug) cout << uistr << "Vmax:\t" << vmax_val << "\t| " << vmax << endl;

	bytesinit.push_back(0x0d); // vmax
	bytesinit.push_back(vmax % 256);
	bytesinit.push_back(vmax / 256);

	vmin_val = spinVmin->value();
	vmin = vmin_val / 2.5 * (pow(2,16) - 1);
	if (debug) cout << uistr << "Vmin:\t" << vmin_val << "\t| " << vmin << endl;

	bytesinit.push_back(0x0e); // vmin
	bytesinit.push_back(vmin % 256);
	bytesinit.push_back(vmin / 256);

	bytesinit.push_back(0x12); // reset dac

	vrst_val = spinVrst->value();
	vrst = vrst_val / 2.5 * (pow(2,16) - 1);
	if (debug) cout << uistr << "Vrst:\t" << vrst_val << "\t| " << vrst << endl;

	bytesinit.push_back(0x10); // vrst
	bytesinit.push_back(vrst % 256);
	bytesinit.push_back(vrst / 256);
	
	vbias_val = spinVbias->value();
	vbias = vbias_val / 2.5 * (pow(2,16) - 1);
	if (debug) cout << uistr << "Vbias:\t" << vbias_val << "\t| " << vbias << endl;
	
	bytesinit.push_back(0x11); // vbias
	bytesinit.push_back(vbias % 256);
	bytesinit.push_back(vbias / 256);

	bytesinit.push_back(0x00);
	bytesinit.push_back(0x01); // write
	bytesinit.push_back(0x00); // memory 0

	timeout_val = spinTimeout->value();
	if (debug) cout << uistr << "Timeout:\t" << timeout_val << endl;

	frameCount = 0;
	frameMax = spinFrames->value();
	if (debug) cout << uistr << "Frames:\t" << frameMax << endl;

	string program = "";

	if (idxProgram == idxImage) {

		if (imageMode == IMAGE_FAST && !cds)
			
			program = asmImage;

		else if (imageMode == IMAGE_FAST && cds)
			
			program = asmCDSImage;

		else if (imageMode == IMAGE_SLOW && !cds)
			
			program = asmSlowImage;

		else if (imageMode == IMAGE_SLOW && cds)
			
			program = asmCDSSlowImage;

		else if (imageMode == IMAGE_FPN)

			program = asmFPNImage;

	}
	else if (idxProgram == idxRCR)
		
		program = asmRCR;

	else if (idxProgram == idxEdge)
		
		program = asmEdge;

	else if (idxProgram == idxMedian && !cds)
		
		program = asmMedian;

	else if (idxProgram == idxMedian && cds)
		
		program = asmCDSMedian;

	else if (idxProgram == idxDCT)
		
		program = asmDCT;

	else if (idxProgram == idxHistogram && !cds)
		
		program = asmHistogram;

	else if (idxProgram == idxHistogram && cds)
		
		program = asmCDSHistogram;

	else if (idxProgram == idxTracking) {
		
		     if (trackMode == TRACK_REGULAR && !cds)

			program = asmTrack;

		else if (trackMode == TRACK_REGULAR && cds)
			
			program = asmCDSTrack;

		else if (trackMode == TRACK_EMBED && !cds)
			
			program = asmEmbedTrack;

		else if (trackMode == TRACK_EMBED && cds)
			
			program = asmCDSEmbedTrack;

		else if (trackMode == TRACK_TRAJL && !cds)
			
			program = asmLinearTrajTrack;
		
		else if (trackMode == TRACK_TRAJNL && !cds)
			
			program = asmNonlinearTrajTrack;
		
		else if (trackMode == TRACK_TRAJL && cds)
			
			program = asmCDSLinearTrajTrack;
		
		else if (trackMode == TRACK_TRAJNL && cds)
			
			program = asmCDSNonlinearTrajTrack;
		
		else if (trackMode == TRACK_RPM)

			program = asmRPM;

	}
	else if (idxProgram == idxAER)
		
		program = asmAER;

	else if (idxProgram == idxBench)
		
		program = asmBench;

	readASMhex(&program, &bytesprogram);

	     
	vector<uint>::iterator it, itn;
	     
	if (idxProgram == idxEdge)
	{
		it  = threshLinesEdge.begin(); itn = threshLinesEdge.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_REGULAR && !cds)
	{
		it  = threshLinesTrack.begin(); itn = threshLinesTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_REGULAR && cds)
	{
		it  = threshLinesCDSTrack.begin(); itn = threshLinesCDSTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_EMBED)
	{
		it  = threshLinesEmbedTrack.begin(); itn = threshLinesEmbedTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_TRAJL && !cds)
	{
		it  = threshLinesLinearTrajTrack.begin(); itn = threshLinesLinearTrajTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_TRAJL && cds)
	{
		it  = threshLinesCDSLinearTrajTrack.begin(); itn = threshLinesCDSLinearTrajTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_TRAJNL && !cds)
	{
		it  = threshLinesNonlinearTrajTrack.begin(); itn = threshLinesNonlinearTrajTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_TRAJNL && cds)
	{
		it  = threshLinesCDSNonlinearTrajTrack.begin(); itn = threshLinesCDSNonlinearTrajTrack.end();
	}
	else if (idxProgram == idxTracking && trackMode == TRACK_RPM)
	{
		it  = threshLinesRPM.begin(); itn = threshLinesRPM.end();
	}
	for (; it != itn; ++it)
	{
		bytesprogram.at(*it * 2 + 2) = spinThreshold->value(); // offset of 2 bytes due to size bytes
	}

	// join vectors
	bytestotal.insert(bytestotal.end(),    bytesinit.begin(),    bytesinit.end());
	bytestotal.insert(bytestotal.end(), bytesprogram.begin(), bytesprogram.end());

	// data bytes
	bytestotal.push_back(0x02); // start

	// out instructions
	unsigned int insOut = 0x13, insBp = 0x09, insRepeat = 0x18, *bytes = bpImage;
	unsigned int bpHistStart = 0, bpHistOut = 0, bpTrackOut = 0; // bpTrackStart = 0;

	if (idxProgram == idxBench)
		insOut = 0x17;

	else if (cds && parallel)
		insOut = 0x16;
	
	else if (cds && !parallel)
		insOut = 0x14;
	
	else if (!cds && parallel)
		insOut = 0x15;
	
	else
		insOut = 0x13;

	if (cds)
		insRepeat = 0x19;
	
	else
		insRepeat = 0x18;

	if (idxProgram == idxBench)

		bytesimage.push_back(insOut);
	
	else if (cds)
	{
		if (idxProgram == idxImage)
		{
			if (imageMode == IMAGE_FAST)
				bytes = bpCDSImage;
			
			else if (imageMode == IMAGE_SLOW)
				bytes = bpCDSSlowImage;
			
			else if (imageMode == IMAGE_FPN)
				bytes = bpFPNImage;
		}
		
		else if (idxProgram == idxMedian)
		{
			bytes = bpCDSMedian;

			// 
			// NEED TO UPDATE FOR CDS:
			//
			// skip the original image, just show compressed
			//bytesimage.push_back(insBp);
			//bytesimage.push_back(*(bytes+0) % 256);
			//bytesimage.push_back(*(bytes+0) / 256);
		}
		
		else if (idxProgram == idxHistogram)
		{
			bytes = bpCDSHistogram;
			bpHistStart = bpCDSHistogram[0];
			bpHistOut   = bpCDSHistogram[2];
		}
		
		else if (idxProgram == idxTracking)
		{
			if (trackMode == TRACK_REGULAR)
				bytes = bpCDSTrack;
			
			else if (trackMode == TRACK_EMBED)
				bytes = bpCDSEmbedTrack;
			
			else if (trackMode == TRACK_TRAJL)
				bytes = bpCDSLinearTrajTrack;
			
			else if (trackMode == TRACK_TRAJNL)
				bytes = bpCDSNonlinearTrajTrack;
			
			else if (trackMode == TRACK_RPM)
				bytes = bpCDSRPM;

			// bpTrackStart = *(bytes+0);
			bpTrackOut   = *(bytes+2);
		}

		else if (idxProgram == idxAER)
		{
			bytes = bpAER;
		}

		bytesimage.push_back(insOut);
		bytesimage.push_back(*(bytes+1) % 256);
		bytesimage.push_back(*(bytes+1) / 256);
		bytesimage.push_back(*(bytes+2) % 256);
		bytesimage.push_back(*(bytes+2) / 256);
		
	}
	else if (!cds)
	{
		if (idxProgram == idxImage)
		{
			if (imageMode == IMAGE_FAST)
				
				bytes = bpImage;
			
			else if (imageMode == IMAGE_SLOW)
				
				bytes = bpSlowImage;
		}
		
		else if (idxProgram == idxRCR)
			
			bytes = bpRCR;
		
		else if (idxProgram == idxEdge)
			
			bytes = bpEdge;
		
		else if (idxProgram == idxMedian)
		{
			bytes = bpMedian;

			// skip the original image, just show compressed
			bytesimage.push_back(insBp);
			bytesimage.push_back(*(bytes+1) % 256);
			bytesimage.push_back(*(bytes+1) / 256);
		}

		else if (idxProgram == idxDCT)
			
			bytes = bpDCT;
		
		else if (idxProgram == idxHistogram)
		{
			bytes = bpHistogram;
			bpHistStart = 0;
			bpHistOut   = bpHistogram[0];
		}
		
		else if (idxProgram == idxTracking)
		{
			if (trackMode == TRACK_REGULAR)
				bytes = bpTrack;
			
			else if (trackMode == TRACK_EMBED)
				bytes = bpEmbedTrack;
			
			else if (trackMode == TRACK_TRAJL)
				bytes = bpLinearTrajTrack;
			
			else if (trackMode == TRACK_TRAJNL)
				bytes = bpNonlinearTrajTrack;
			
			// bpTrackStart = 0;
			bpTrackOut   = bytes[0];
		}
		
		else if (idxProgram == idxBench)
			
			bytes = bpBench;
		
		bytesimage.push_back(insOut);
		bytesimage.push_back(*(bytes+0) % 256);
		bytesimage.push_back(*(bytes+0) / 256);
	}



	if (idxProgram == idxImage && imageMode == IMAGE_FAST)
	{
		bytesimage.push_back(insBp);
		bytesimage.push_back(*(bytes+0) % 256);
		bytesimage.push_back(*(bytes+0) / 256);
	
	}
	else if (idxProgram == idxHistogram)
	{
		// LSB
		bytesdata.push_back(insOut);
		bytesdata.push_back(bpHistOut % 256);
		bytesdata.push_back(bpHistOut / 256);
		
		// MSB
		bytesdata.push_back(insOut);
		bytesdata.push_back(bpHistOut % 256);
		bytesdata.push_back(bpHistOut / 256);
		
		bytesdata.push_back(insBp);
		bytesdata.push_back(bpHistStart % 256);
		bytesdata.push_back(bpHistStart / 256);
	
	}
	else if (idxProgram == idxTracking && (trackMode == TRACK_REGULAR || trackMode == TRACK_RPM))
	{
		bytesdata.push_back(insOut);
		bytesdata.push_back(bpTrackOut % 256);
		bytesdata.push_back(bpTrackOut / 256);

		// seems unnecessary
		//bytesdata.push_back(insBp);
		//bytesdata.push_back(bpTrackStart % 256);
		//bytesdata.push_back(bpTrackStart / 256);
	}

	if (repeatRecord)
	{
		bytesrepeat.push_back(insRepeat);
		bytesrepeat.push_back(spinFrames->value() % 256);
		bytesrepeat.push_back(spinFrames->value() / 256);
	}

   
	if (debug) 
	{
		cout << uistr << "Program:\t" << comboProgram->currentText().toStdString() << endl;
		
		(cds) ? 
			cout << uistr << "Breakpoint:\t" << *(bytes+0) << ", " << *(bytes+1) << ", " << *(bytes+2) << endl :
			cout << uistr << "Breakpoint:\t" << *(bytes+0) << endl;
		
		unsigned n;
		
		cout << endl;
		cout << uistr << "Init Bytes (length " << bytesinit.size() << "): " << endl;
		for (vector<uint>::iterator it = bytesinit.begin(); it != bytesinit.end(); ++it)
		{
			renamer.str(""); renamer.clear();
			renamer << hex << *it;
			renamer >> n;
			bitset<8> b(n);
			cout << boost::format("\t%3d : 0x%02x : %s %s\n") % *it % *it % b.to_string() % decodeASMhex(*it);
		}
		cout << endl;
		cout << uistr << "Program Bytes (length " << bytesprogram.size() << "): " << endl;
		for (vector<uint>::iterator it = bytesprogram.begin(); it != bytesprogram.end(); ++it)
		{
			renamer.str(""); renamer.clear();
			renamer << hex << *it;
			renamer >> n;
			bitset<8> b(n);
			cout << boost::format("\t%3d : 0x%02x : %s\n") % *it % *it % b.to_string();
		}
		cout << endl;
		cout << uistr << "Video Bytes (length " << bytesimage.size() << "): " << endl;
		for (vector<uint>::iterator it = bytesimage.begin(); it != bytesimage.end(); ++it)
		{
			renamer.str(""); renamer.clear();
			renamer << hex << *it;
			renamer >> n;
			bitset<8> b(n);
			cout << boost::format("\t%3d : 0x%02x : %s %s\n") % *it % *it % b.to_string() % decodeASMhex(*it);
		}
		cout << endl;
		if (idxProgram == idxHistogram || idxProgram == idxTracking)
		{
			cout << uistr << "Data Bytes (length " << bytesdata.size() << "): " << endl;
			for (vector<uint>::iterator it = bytesdata.begin(); it != bytesdata.end(); ++it)
			{
				renamer.str(""); renamer.clear();
				renamer << hex << *it;
				renamer >> n;
				bitset<8> b(n);
				cout << boost::format("\t%3d : 0x%02x : %s %s\n") % *it % *it % b.to_string() % decodeASMhex(*it);
			}
			cout << endl;
		}
		if (repeatRecord)
		{
			cout << uistr << "Repeat Bytes (length " << bytesrepeat.size() << "): " << endl;
			for (vector<uint>::iterator it = bytesrepeat.begin(); it != bytesrepeat.end(); ++it)
			{
				renamer.str(""); renamer.clear();
				renamer << hex << *it;
				renamer >> n;
				bitset<8> b(n);
				cout << boost::format("\t%3d : 0x%02x : %s %s\n") % *it % *it % b.to_string() % decodeASMhex(*it);
			}
			cout << endl;
		}
	}

	if (!dummy)
	{
	   	if (camera.connect(strdup(serial.c_str()), timeout_val))
	   	{
			cout << uistr << "Apply failed. Please run Setup and ensure PIRANHA is connected" << endl;
			btnStart->setEnabled(false);
			btnStop->setEnabled(false);
			//exit();
		}

		else
		{
			if (!camera.write(&vector<uint8_t>(bytestotal.begin(), bytestotal.end())[0], bytestotal.size()))
			{
				if (!playback) 
					
					btnStart->setEnabled(true);
				
				btnStop->setEnabled(true);
		
				if (debug) cout << uistr << "Connection Established" << endl;
			}
	   	}
		
	}

	else
	{
		btnStart->setEnabled(true);
		btnStop->setEnabled(true);
	}
}

void UIpiranha::start()
{
	if (debug) cout << uistr << "Start" << endl;
	// need to recreate this everytime. once stopped, this is destroyed
	playback = new UIplayback;
	connect(this,     SIGNAL(abortCapture()), playback, SLOT(abort()),      Qt::QueuedConnection);
	connect(playback, SIGNAL(updateNeeded()), this,     SLOT(printFrame()), Qt::QueuedConnection);
	playback->setTimeout(timeout_val + 1);
	
	QThreadPool::globalInstance()->setMaxThreadCount(1); // prevent multiple attempts to access the device
	QThreadPool::globalInstance()->start(playback);
	btnStart->setEnabled(false);
}

void UIpiranha::stop()
{
	if (debug) cout << uistr << "Stop" << endl;
	emit abortCapture();
	btnStart->setEnabled(true);
	coorList.assign(coorMax, dataCoor(0,0));

	cout << uistr << boost::format("FPS: %1.2f (avg), [%1.2f (min) %1.2f (max)]") % fpsavg % fpsmin % fpsmax << endl;
	fpsmin = 0; fpsmax = 0; fpscount = 0, fpstotal = 0;

	if (recording) record_stop();
}

void UIpiranha::reset()
{
	if (debug) cout << uistr << "Reset" << endl;
	
	camera.disconnect();
	
	fpsavg = 0; fpsmin = 0; fpsmax = 0; fpscount = 0, fpstotal = 0, errorCount = 0;
	lcdFPS->setText("0.00");
	
	if (stretch)
	{
		imgStretch = Mat(imgStretch.size(), CV_8UC3, Scalar(0,0,0));
		lblImage->setPixmap(QPixmap::fromImage(mat2qimage(imgStretch)));
	}
	else
	{
		img = Mat(img.size(), CV_8UC3, Scalar(0,0,0));
		lblImage->setPixmap(QPixmap::fromImage(mat2qimage(img)));
	}
	
	coorList.assign(coorMax, dataCoor(0,0));
	predictList.assign(predictMax, dataCoor(0,0));
}

void UIpiranha::export_image()
{
	if (debug) cout << uistr << "Export Image" << endl;

	renamer.str(""); renamer.clear();
	renamer << lastDir.toStdString();
	renamer << boost::format("%s%s%s-%dus-%dms") % programNames[idxProgram] % ((cds) ? "-cds-" : "-")  % ((repeatRecord) ? "repeat" : ((parallel) ? "parallel" : "normal")) % spinTint->value() % spinTimeout->value();
	
	if (idxProgram == idxTracking || idxProgram == idxEdge)
	{
		renamer << "-"; renamer << boost::format("%dt") % spinThreshold->value();
	}
	
	QString filter = tr("PGM files (*.pgm);;PNG files (*.png)");
	QString ext;
	QString filename = QFileDialog::getSaveFileName(this, tr("Export File"), QString::fromStdString(renamer.str()), filter, &ext);
	ext = ext.remove(")").remove(0, ext.lastIndexOf(".")).toLower();
	if (filename.endsWith(ext))
		ext = "";
	Mat *imgsave = &img;

	if (stretch)
		*imgsave = imgStretch;

	if (filename != "") {
		
		lastDir = filename.section('/', 0, -2) + "/";
		if (debug) cout << "Remembering directory: " << lastDir.toStdString() << endl;
		
		if (ext == ".pgm") {
			
			if (imwrite(filename.toStdString() + ext.toStdString(), *imgsave, paramspxm))
				
				cout << uistr << "Exported " << filename.toStdString() << ext.toStdString() <<  endl;
			
			if (idxProgram == idxTracking && trackMode != TRACK_EMBED && trackMode != TRACK_TRAJL && trackMode != TRACK_TRAJNL)
			{
				// also save data pgm
				if (imwrite(filename.toStdString() + "_data" + ext.toStdString(), trackData, paramspxm))
					
					cout << uistr << "Exported " << filename.toStdString() << "_data" << ext.toStdString() << endl;
			}
			else if (idxProgram == idxHistogram)
			{
				// also save the LSB and MSB data pgms
				if (imwrite(filename.toStdString() + "_lsb" + ext.toStdString(), histLSB, paramspxm))
					
					cout << uistr << "Exported " << filename.toStdString() << "_lsb" << ext.toStdString() << endl;
				
				if (imwrite(filename.toStdString() + "_msb" + ext.toStdString(), histMSB, paramspxm))
					
					cout << uistr << "Exported " << filename.toStdString() << "_msb" << ext.toStdString() << endl;
			}
		}
		else if (ext == ".png") {
			
			if (imwrite(filename.toStdString() + ext.toStdString(), *imgsave, paramspng))
				cout << uistr << "Exported " << filename.toStdString() << ext.toStdString() <<  endl;
			
			if (idxProgram == idxTracking && trackMode != TRACK_EMBED && trackMode != TRACK_TRAJL && trackMode != TRACK_TRAJNL)
			{
				// also save data pgm
				if (imwrite(filename.toStdString() + "_data" + ext.toStdString(), trackData, paramspxm))
					cout << uistr << "Exported " << filename.toStdString() << "_data" << ext.toStdString() << endl;
			}
		}
	}
}

void UIpiranha::record_start()
{
	
	if (debug) cout << uistr << "Export Video" << endl;

	frameMax = spinFrames->value();
	frameSuccess = 0;
	
	if (repeatRecord)
	{
		unsigned int insRepeat = 0x18;
		
		if (cds)
			insRepeat = 0x19;
		
		else
			insRepeat = 0x18;

		bytesrepeat.clear();
		bytesrepeat.push_back(insRepeat);
		bytesrepeat.push_back(frameMax % 256);
		bytesrepeat.push_back(frameMax / 256);
	}

	if (frameMax != 0)
	{
		renamer.str(""); renamer.clear();
		renamer << lastDir.toStdString();
		renamer << boost::format("%s%s%s-%dus-%dms") % programNames[idxProgram] % ((cds) ? "-cds-" : "-")  % ((repeatRecord) ? "repeat" : ((parallel) ? "parallel" : "normal")) % spinTint->value() % spinTimeout->value();
	
		if (idxProgram == idxTracking || idxProgram == idxEdge)
		{
			renamer << "-"; renamer << boost::format("%dt") % spinThreshold->value();
		}
		
		QString filter = tr("Video Files (*.avi)");
		QString ext;

		QString filename = QFileDialog::getSaveFileName(this, tr("Export File"), QString::fromStdString(renamer.str()), filter, &ext);
		string videoName;
		
		ext = ext.remove(")").remove(0, ext.lastIndexOf(".")).toLower();
		
		if (filename.endsWith(ext))
			
			ext = "";

		videoName = filename.toStdString() + ext.toStdString();
		
		Mat *imgsave = &img;

		if (stretch && encoderStretchable)
			
			*imgsave = imgStretch;

		if (filename != "") {
			
			lastDir = filename.section('/', 0, -2) + "/";
			if (debug) cout << "Remembering directory: " << lastDir.toStdString() << endl;
			
			cout << uistr << "Recording to " << videoName << endl;
			
			encoderFPS = spinFPS->value();
			
			if (encoderFPS == 0)
			{
				encoderFPS = 30;
				encoderTranscode = true;
			}
			
			videoOut.open(videoName, encoderCodec, encoderFPS, imgsave->size(), true);

			recording = true;
			btnRecordStop->setEnabled(true);
			btnRecordStart->setEnabled(false);
			barRecord->setMaximum(frameMax);
			barRecord->reset();

			fpstotal = 0;
			fps = 0;
			barUpdate = (int) frameMax / barIncrements;

			if (debug) cout << uistr << "Updating progress bar every " << barUpdate << " frames" << endl;
			
			//imgGray = Mat(Size(80,64), CV_8UC1, Scalar(0));

			Mat framesRecord[frameMax+1], framesRecordColor[frameMax+1], framesRecordData[frameMax+1], framesRecordStretched[frameMax+1];

			for (frameCount = 0; frameCount < frameMax; ++frameCount)
			{
				framesRecord[frameCount]      = Mat(Size(80,64), CV_8UC1, Scalar(0));
				framesRecordColor[frameCount] = Mat(Size(80,64), CV_8UC3, Scalar(0,0,0));
			
				if (idxProgram == idxTracking && trackMode != TRACK_EMBED)

					framesRecordData[frameCount] = Mat(Size(80,64), CV_8UC1, Scalar(0));

				if (stretch && encoderStretchable)

					framesRecordStretched[frameCount] = Mat(Size(80 * stretchBy, 64 * stretchBy), CV_8UC3, Scalar(0,0,0));
			}

			timeold = chrono::high_resolution_clock::now();

			if (repeatRecord)
			{
				if(camera.repeatCommand(&vector<uint8_t>(bytesrepeat.begin(), bytesrepeat.end())[0], bytesrepeat.size()))
				{
					cout << uistr << "ERROR: Couldn't retrieve bulk request packet" << endl;
					record_stop();
					emit abortCapture();
				}
				
				else
					cout << uistr << "Repeat request started" << endl;
			}
			
			for (frameCount = 0; frameCount < frameMax; ++frameCount)
			{
				if (acquireImage(&framesRecord[frameCount]))
					
					cout << uistr << "ERROR: Frame error detected" << endl;

				else 
				{
					frameSuccess++;
					
					if ((frameCount+1) % barUpdate == 0)
					
						barRecord->setValue(frameCount+1);

				}
			}
			
			timeimg = chrono::high_resolution_clock::now();
			
			for (frameCount = 0; frameCount < frameSuccess; ++frameCount)
			{
				cv::cvtColor(framesRecord[frameCount],  framesRecordColor[frameCount], CV_GRAY2RGB);

				     if (idxProgram == idxTracking && trackMode == TRACK_EMBED)
				{
					trackDataPlotEmbedded(&framesRecordColor[frameCount]);
				}
				else if (idxProgram == idxTracking && (trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL))
				{
					trackDataPlotTrajectory(&framesRecordColor[frameCount]);
				}
				else if (idxProgram == idxTracking)
				{
					trackDataPlot(&framesRecordColor[frameCount], &framesRecordData[frameCount]);
				}
				
				if (flipimg)
				
					cv::flip(framesRecordColor[frameCount], framesRecordColor[frameCount], 0); // vertical flip code

				if (stretch && encoderStretchable)
				{
					cv::resize(framesRecordColor[frameCount], framesRecordStretched[frameCount], framesRecordStretched[frameCount].size(), stretchBy, stretchBy, CV_INTER_NN);

					if (videoOut.isOpened() && recording)
						videoOut << framesRecordStretched[frameCount];
				}
				else
				{
					if (videoOut.isOpened() && recording)
						videoOut << framesRecordColor[frameCount];
				}
	
			}

			timenow = chrono::high_resolution_clock::now();
			
			timespan = chrono::duration_cast<chrono::duration<double>>(timeimg - timeold);

			timeproc = chrono::duration_cast<chrono::duration<double>>(timenow - timeimg);
			
			fpsavg = frameSuccess / timespan.count();
			int intFpsAvg = (int) fpsavg;

			cout << uistr << "Recorded " << frameCount << " frames in " << timespan.count() << "s with " << timeproc.count() << "s of post processing" << endl;
			cout << uistr << boost::format("Average FPS: %1.2f") % fpsavg << endl;

			renamer.str(""); renamer.clear();
			renamer << boost::format("%s-%dfps") % filename.toStdString() % intFpsAvg;
			if (frameSuccess != frameMax) 
				renamer << "-incomplete";
			string renamedFile = renamer.str();
			renamer << ext.toStdString();
			string renamedFileExt = renamer.str();
			renamer.str(""); renamer.clear();

			if (!rename(strdup(videoName.c_str()), strdup(renamedFileExt.c_str())))
				cout << uistr << "Renamed to " << renamedFileExt << endl;

			// transcodes video to dynamic fpsavg framerate instead of predefined fixed e.g. 30 fps
			if (encoderTranscode)
			{
				renamer << renamedFile << "-transcoded" << ext.toStdString();
				string transcodedVideoName = renamer.str();
				VideoCapture inVid(renamedFileExt);
				VideoWriter outVid(transcodedVideoName, encoderCodec, intFpsAvg, imgsave->size(), true);
				Mat inFrame;

				encoderTranscode = false;

				if (!inVid.isOpened())
					cout << uistr << "ERROR: Couldn't open original video for transcode" << endl;
				
				else
				{
					while(1)
					{
						inVid >> inFrame;
						
						if (inFrame.empty())
							break;

						outVid << inFrame;
					}
					cout << uistr << "Completed transcoding" << endl;
					cout << uistr << "  Original  : " << videoName << " @ " << encoderFPS << " fps" << endl;
					cout << uistr << "  Transcoded: " << transcodedVideoName << " @ " << intFpsAvg << " fps" << endl;
				}
			}
			
			btnRecordStart->setEnabled(true);
			btnRecordStop->setEnabled(false);
			recording = false;

			stop();
			
		}
		else
			cout << uistr << "No filename specified, aborting record" << endl;
	}
	else
	{
		cout << uistr << "ERROR: Recording requires a non-zero number of frames to capture" << endl;
		tabConfig->setCurrentIndex(1);
		spinFrames->setFocus();
		spinFrames->selectAll();
	}
}

void UIpiranha::record_stop()
{
	if (!videoOut.isOpened())
	{
		cout << "Error writing video file" << endl;
		return;
	}
	recording = false;
	videoOut.release();
	cout << "Recording Complete" << endl;
	btnRecordStop->setEnabled(false);
	btnRecordStart->setEnabled(true);
}


int UIpiranha::acquireImage(Mat* imgGray)
{
	if (!dummy)
	{
		if (repeatRecord && recording)
		{
			if (camera.repeatData(imgGray))
			{
				cout << uistr << "ERROR: Couldn't retrieve bulk request packet" << endl;
				emit abortCapture();
				btnStart->setEnabled(true);
				return EXIT_FAILURE;
			}
		}
		
		else if (idxProgram == idxTracking && trackMode == TRACK_REGULAR && !parallel)
		{
			if (camera.captureData(&vector<uint8_t>(bytesimage.begin(), bytesimage.end())[0], bytesimage.size(), &vector<uint8_t>(bytesdata.begin(), bytesdata.end())[0], bytesdata.size(), imgGray, &trackData))
			{
				cout << uistr << "ERROR: Tracking aborted. Try increasing USB Timeout" << endl;
				emit abortCapture();
				btnStart->setEnabled(true);
				return EXIT_FAILURE;
			}
		}
		else if (idxProgram == idxTracking && trackMode == TRACK_REGULAR && parallel)
		{
			if (camera.captureData(&vector<uint8_t>(bytesdata.begin(), bytesdata.end())[0], bytesdata.size(), &vector<uint8_t>(bytesimage.begin(), bytesimage.end())[0], bytesimage.size(), &trackData, imgGray))
			{
				cout << uistr << "ERROR: Tracking aborted. Try increasing USB Timeout" << endl;
				emit abortCapture();
				btnStart->setEnabled(true);
				return EXIT_FAILURE;
			}
		}
		else if (idxProgram == idxHistogram)
		{
			if (camera.captureHistogram(&vector<uint8_t>(bytesimage.begin(), bytesimage.end())[0], bytesimage.size(), &vector<uint8_t>(bytesdata.begin(), bytesdata.end())[0], bytesdata.size(), imgGray, &histLSB, &histMSB))
			{
				cout << uistr << "ERROR: Histogram capture aborted" << endl;
				emit abortCapture();
				btnStart->setEnabled(true);
				return EXIT_FAILURE;
			}
		}
		else
		{
			if (camera.capture(&vector<uint8_t>(bytesimage.begin(), bytesimage.end())[0], bytesimage.size(), imgGray, transformimg))
			{
				cout << uistr << "WARNING: Image error detected" << endl;


				if (errorCount++ == errorMax)
				{
					errorCount = 0;
					emit abortCapture();
					btnStart->setEnabled(true);

					cout << uistr << "Exceeded max amount of errors (" << errorMax << "). Aborting capture" << endl;
				}
				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		if (camera.captureRandom(imgGray))
		{
			cout << uistr << "ERROR: Could not random capture" << endl;
			emit abortCapture();
			btnStart->setEnabled(true);
			return EXIT_FAILURE;
		}	
	}
	
	return EXIT_SUCCESS;
}

void UIpiranha::printFrame()
{
	
	imgGray = Mat(Size(80,64), CV_8UC1, Scalar(0));

	if (acquireImage(&imgGray))
		return;

	timeold = timenow;
	timenow = chrono::high_resolution_clock::now();
	timespan = chrono::duration_cast<chrono::duration<double>>(timenow - timeold);
	fps = 1/(timespan.count());

	if (fps < fpsmin || fpsmin == 0)
		fpsmin = fps;
	
	if (fps > fpsmax)
		fpsmax = fps;

	if ((++fpscount) == fpsavgcount)
	{
		fpsavg = fpstotal/fpsavgcount;
		fpscount = 0;
		fpstotal = 0;
	}
	else
		fpstotal += fps;

	lcdFPS->setText(QString("%1").arg(fpsavg, 0, 'f', 2));

	cv::cvtColor(imgGray, img, CV_GRAY2RGB);

	if (idxProgram == idxTracking && trackMode == TRACK_EMBED)

		trackDataPlotEmbedded(&img);

	else if (idxProgram == idxTracking && (trackMode == TRACK_TRAJL || trackMode == TRACK_TRAJNL))

		trackDataPlotTrajectory(&img);

	else if (idxProgram == idxTracking)

		trackDataPlot(&img, &trackData);

	
	if (flipimg)
	
		cv::flip(img, img, 0); // vertical flip code
	//	cv::flip(img, img, 1); // horizontal flip code

	if (stretch)
	{
		cv::resize(img, imgStretch, imgStretch.size(), stretchBy, stretchBy, CV_INTER_NN);
		lblImage->setPixmap(QPixmap::fromImage(mat2qimage(imgStretch)));
	}
	else
		
		lblImage->setPixmap(QPixmap::fromImage(mat2qimage(img)));
	
	// 
	//  old code for limiting the # of frames printed. not much use anymore with faster int times
	//
	
	//if (frameMax != 0)
	//{
	//	if (++frameCount > frameMax)
	//	{
	//		if (debug)
	//			cout << uistr << "Capture complete" << endl;
	//		
	//		frameCount = 0;
	//		emit abortCapture();
	//		btnStart->setEnabled(true);
	//		btnStop->setEnabled(false);

	//		if (recording)
	//			record_stop();
	//	}
	//	//else if (debug)
	//	//	cout << uistr << "Frame " << frameCount << endl;
	//}
}

void UIpiranha::readASMhex(string* filename, vector<uint>* bytes)
{
	string line, wordlsb, wordmsb;
	uint bytelsb, bytemsb, lines = 0;
	ifstream infile(programdir + "/" + *filename);
	bytes->push_back(0); bytes->push_back(0); // placeholder for size bytes

	while (getline(infile, line))
	{
		wordlsb = line.substr(8,8);
		wordmsb = line.substr(0,8);
		bytelsb = (uint) bitset<8>(wordlsb).to_ulong();
		bytemsb = (uint) bitset<8>(wordmsb).to_ulong();

		bytes->push_back(bytelsb);
		bytes->push_back(bytemsb);
		++lines;
	}
	bytes->at(0) = (uint) lines % 256;
	bytes->at(1) = (uint) lines / 256;

	if (debug) cout << uistr << "Read " << lines << " lines from " << programdir << "/" << *filename << endl;
}

string UIpiranha::decodeASMhex(int ins)
{
	string insDecode = "";
	
	if (decodeWrite)
	{
		switch (ins)
		{
			case 0x00:
				insDecode = "  Memory 0"; break;
			case 0x01:
				insDecode = "  Memory 1"; break;
		}

		decodeWrite = false;
	}
	
	else if (decodeBP)
	{
		insDecode = "  BP Lower";
		decodeBP = false;
		decodeBPUpper = true;
	}
	
	else if (decodeBPUpper)
	{
		insDecode = "  BP Upper";
		decodeBPUpper = false;
	}

	else if (decodeCDSBP)
	{
		insDecode = "  BP Lower CDS";
		decodeCDSBP = false;
		decodeCDSBPUpper = true;
	}
	
	else if (decodeCDSBPUpper)
	{
		insDecode = "  BP Upper CDS";
		decodeCDSBPUpper = false;
		decodeBP = true;
	}

	else if (decodeCount)
	{
		insDecode = "  # Frames Lower";
		decodeCount      = false;
		decodeCountUpper = true;
	}
	
	else if (decodeCountUpper)
	{
		insDecode = "  # Frames Upper";
		decodeCount      = false;
		decodeCountUpper = false;
	}

	else
	{
		switch (ins)
		{
			case 0x01:
				insDecode = "Write";
				decodeWrite = true;
				break;
			
			case 0x02:
				insDecode = "Start"; break;
			
			case 0x03:
				insDecode = "Stop"; break;
			
			case 0x04:
				insDecode = "Change"; break;
			
			case 0x05:
				insDecode = "Read"; break;
			
			case 0x06:
				insDecode = "Step"; break;
			
			case 0x07:
				insDecode = "Output"; break;
			
			case 0x08:
				insDecode = "PC"; break;
			
			case 0x09:
				insDecode = "Breakpoint";
				decodeBP = true;
				break;
			
			case 0x0a:
				insDecode = "Reset"; break;
			
			case 0x0b:
				insDecode = "Set Trst"; break;
			
			case 0x0c:
				insDecode = "Set Tint"; break;
			
			case 0x0d:
				insDecode = "Set Vmax"; break;
			
			case 0x0e:
				insDecode = "Set Vmin"; break;
			
			case 0x10:
				insDecode = "Set Vrst"; break;
			
			case 0x11:
				insDecode = "Set Vbias"; break;
			
			case 0x12:
				insDecode = "Reset SDAC"; break;
			
			case 0x13:
				insDecode = "Image";
				decodeBP = true;
				break;
			
			case 0x14:
				insDecode = "CDS Image";
				decodeCDSBP = true;
				break;
			
			case 0x15:
				insDecode = "Image Parallel";
				decodeBP = true;
				break;
			
			case 0x16:
				insDecode = "CDS Image Parallel";
				decodeCDSBP = true;
				break;
			
			case 0x17:
				insDecode = "Benchmark"; break;
			
			case 0x18:
				insDecode = "Repeat"; 
				decodeCount = true;
				break;
			
			case 0x19:
				insDecode = "Repeat CDS"; 
				decodeCount = true;
				break;
		}
	}

	if (insDecode != "")
		insDecode = ": " + insDecode;

	return insDecode;
}

QImage UIpiranha::mat2qimage(const Mat& mat)
{
	//return QImage((const unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_Indexed8);
	return QImage((unsigned char*) mat.data, mat.cols, mat.rows, mat.step1(), QImage::Format_RGB888);
}

void UIpiranha::trackDataPlot(Mat* img, Mat* dataTrack)
{	
	for (vector<dataCoor>::iterator it = coorList.begin(); it != coorList.end(); ++it)
	{
		if (it->y || it->x)
		{
			img->at<Vec3b>(it->y * img->cols + it->x)[0] = colorLine.val[0];
			img->at<Vec3b>(it->y * img->cols + it->x)[1] = colorLine.val[1];
			img->at<Vec3b>(it->y * img->cols + it->x)[2] = colorLine.val[2];
		}
	}

	unsigned int coorx, coory;
	unsigned int coorcount = 0;
	
	for (unsigned int r = 0; r < 1; ++r)
	{
		for (int c = 0; c < 2; c+=2)
		{
			coorx = dataTrack->data[(8 * r + 1) * dataTrack->cols + c];
			coory = dataTrack->data[(8 * r + 1) * dataTrack->cols + c + 1];
			if (coorx || coory)
			{
				img->at<Vec3b>(coory * img->cols + coorx)[0] = colorTrack.val[0];
				img->at<Vec3b>(coory * img->cols + coorx)[1] = colorTrack.val[1];
				img->at<Vec3b>(coory * img->cols + coorx)[2] = colorTrack.val[2];
				coorList[coorIdx] = dataCoor(coorx, coory);
				coorcount++;
				coorIdx++;
				coorIdx = coorIdx % coorMax;
				//if (debug && coorcount < 80)
				//	cout << "[" << coorx << ", " << coory << "] "; 
			}
			if (trackMode == TRACK_RPM)
			{
				unsigned int rpmx, rpmy, rpmcount, rpmreached = 0;
				rpmx       = dataTrack->data[(8 * r + 2) * dataTrack->cols + 0];
				rpmy       = dataTrack->data[(8 * r + 2) * dataTrack->cols + 1];
				rpmcount   = dataTrack->data[(8 * r + 2) * dataTrack->cols + 2];
				rpmreached = dataTrack->data[(8 * r + 2) * dataTrack->cols + 3];

				//if (rpmreached == 1)
				//{
					cout << "RPM: " << rpmcount << " (" << rpmx << ", " << rpmy << ") : " << rpmreached << endl;
				//}
			}
		}
	}

	if (debug && coorcount > 0)
	{
		//cout << endl;
		cout << uistr << "\rCoordinates loaded: " << coorcount << endl;
	}
}

void UIpiranha::trackDataPlotEmbedded(Mat* img)
{	
	if (coorList.size() > 0)
	{
		
		for (vector<dataCoor>::iterator it = coorList.begin(); it != coorList.end(); ++it)
		{
			if (it->y || it->x)
			{
				img->at<Vec3b>(it->y * img->cols + it->x)[0] = colorLine.val[0];
				img->at<Vec3b>(it->y * img->cols + it->x)[1] = colorLine.val[1];
				img->at<Vec3b>(it->y * img->cols + it->x)[2] = colorLine.val[2];
			}
		}

		coorx = img->at<Vec3b>(0)[0];
		coory = img->at<Vec3b>(1)[0];

		if ((coorx || coory) && (coorx < 80 && coory < 64))
		{
			img->at<Vec3b>(coory * img->cols + coorx)[0] = colorTrack.val[0];
			img->at<Vec3b>(coory * img->cols + coorx)[1] = colorTrack.val[1];
			img->at<Vec3b>(coory * img->cols + coorx)[2] = colorTrack.val[2];
			coorList[coorIdx] = dataCoor(coorx, coory);
			coorIdx++;
			coorIdx = coorIdx % coorMax;

			if (debug && (coorx != coorxold || coory != cooryold)) cout << uistr << "\rCoordinate: (" << coorx << ", " << coory << ")" << endl;

			coorxold = coorx;
			cooryold = coory;
		}
	
	}
	
}

void UIpiranha::trackDataPlotTrajectory(Mat* img)
{	
	if (coorList.size() > 0)
	{
		
		// plot the previous path
		for (vector<dataCoor>::iterator it = coorList.begin(); it != coorList.end(); ++it)
		{
			if (it->y || it->x)
			{
				img->at<Vec3b>(it->y * img->cols + it->x)[0] = colorLine.val[0];
				img->at<Vec3b>(it->y * img->cols + it->x)[1] = colorLine.val[1];
				img->at<Vec3b>(it->y * img->cols + it->x)[2] = colorLine.val[2];
			}
		}

		// centroid
		coorx = img->at<Vec3b>(trackMode == TRACK_TRAJNL ? 0 : 0)[0];
		coory = img->at<Vec3b>(trackMode == TRACK_TRAJNL ? 1 : 1)[0];

		if ((coorx || coory) && (coorx < 80 && coory < 64))
		{
			img->at<Vec3b>(coory * img->cols + coorx)[0] = colorTrack.val[0];
			img->at<Vec3b>(coory * img->cols + coorx)[1] = colorTrack.val[1];
			img->at<Vec3b>(coory * img->cols + coorx)[2] = colorTrack.val[2];
			coorList[coorIdx] = dataCoor(coorx, coory);
			coorIdx++;
			coorIdx = coorIdx % coorMax;

			if (debug && (coorx != coorxold || coory != cooryold)) {

				if (trackMode == TRACK_TRAJL)

					cout << uistr << boost::format("Data: (%2u,%2u) Predicted: (%2u,%2u), (%2u,%2u), (%2u,%2u)")
								% (unsigned int) img->at<Vec3b>(0)[0] // x_i
								% (unsigned int) img->at<Vec3b>(1)[0] // y_i
								% (unsigned int) img->at<Vec3b>(2)[0] // xp0
								% (unsigned int) img->at<Vec3b>(3)[0] // yp0
								% (unsigned int) img->at<Vec3b>(4)[0] // xp1
								% (unsigned int) img->at<Vec3b>(5)[0] // yp1
								% (unsigned int) img->at<Vec3b>(6)[0] // xp2
								% (unsigned int) img->at<Vec3b>(7)[0] // yp2
								;
				
				else if (trackMode == TRACK_TRAJNL)
					
					cout << uistr << boost::format("Data: (%2u,%2u) @ V(%s%2u,%s%2u) + A(%2u,%2u)")
								% (unsigned int) img->at<Vec3b>( 0)[0]  //   x_i
								% (unsigned int) img->at<Vec3b>( 1)[0]  //   y_i
			  % ((unsigned int) img->at<Vec3b>(80)[0] == 1 ? " " : "-") // sdx_i
								% (unsigned int) img->at<Vec3b>( 4)[0]  //  dx_i
			  % ((unsigned int) img->at<Vec3b>(81)[0] == 1 ? " " : "-") // sdy_i
								% (unsigned int) img->at<Vec3b>( 5)[0]  //  dy_i
								% (unsigned int) img->at<Vec3b>(82)[0]  // ddx_i
								% (unsigned int) img->at<Vec3b>(83)[0]  // ddy_i
								;
			
				cout << uistr << boost::format("\t%d / %d") % predictUpdate % predictInterval << endl;
			}

			coorxold = coorx;
			cooryold = coory;
		}
		
		// predicted next (x,y)
		// z memory:
		//   x_center, y_center, x_next0, y_next0, x_next1, y_next1, x_next2, y_next2

		// shaky prediction, only redraw regularly
		
		predictUpdate++;

		for (unsigned int pint = 0; pint < predictMax; pint++)
		{
			if ((predictUpdate % predictInterval) == 0)
			{
				predictUpdate = 0;

				if (trackMode == TRACK_TRAJL) {
					coorx = img->at<Vec3b>(2+pint*2)[0];
					coory = img->at<Vec3b>(3+pint*2)[0];
					predictList[pint] = dataCoor(coorx, coory);
				}
				else if (trackMode == TRACK_TRAJNL) {
					coorx = img->at<Vec3b>(predictTable[pint] + 0)[0];
					coory = img->at<Vec3b>(predictTable[pint] + 1)[0];
					predictList[pint] = dataCoor(coorx, coory);
				}
			}

			// plot the trajectory
			
			for (vector<dataCoor>::iterator it = predictList.begin(); it != predictList.end(); ++it)
			{
				if (it->y || it->x)
				{
					img->at<Vec3b>(it->y * img->cols + it->x)[0] = colorPredict.val[0];
					img->at<Vec3b>(it->y * img->cols + it->x)[1] = colorPredict.val[1];
					img->at<Vec3b>(it->y * img->cols + it->x)[2] = colorPredict.val[2];
				}
			}
		}
	}
}
