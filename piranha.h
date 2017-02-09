#ifndef EXAMPLEPLI892_H
#define EXAMPLEPLI892_H

#include <QtGui>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QT_BEGIN_NAMESPACE

class UIpiranha : public QMainWindow
{

Q_OBJECT

public:
    UIpiranha();
    ~UIpiranha();

private:

    void readASMhex(std::string*, std::vector<uint>*);
    std::string decodeASMhex(int);
    
    QString mu			= QChar(0x03bc);
    QString iconPlay	= QChar(0x23f5);
    QString iconPause	= QChar(0x23f8);
    QString iconStop	= QChar(0x23f9);
    QString iconRecord	= QChar(0x23fa);
    QString iconUpload	= QChar(0x23f6);
    QString iconReset	= QChar(0x2b6f);
    QString iconSave	= QChar(0x1f4be);

    QAction *actionRecord, *actionExport, *actionExit, *actionSetup;
    QWidget *centralwidget;
    
    QFrame *frameVideo;
    QLabel *lcdFPS;
    
    QTabWidget *tabConfig;
    QVBoxLayout *layoutConfig;
    QStackedLayout *layoutOpts;
    QGridLayout *layoutProgram;
    QGridLayout *layoutBlank;
    QGridLayout *layoutImage;
    QGridLayout *layoutTrack;
    QGridLayout *layoutFlags;
    QButtonGroup *grpImage;
    QButtonGroup *grpTrack;
    QButtonGroup *grpFlags;
    QGridLayout *layoutSettings;
    
    QTabWidget *tabActions;
    QGridLayout *layoutActions;
    QGridLayout *layoutRecord;
    
    
    QLabel *lblProgram;
    QComboBox *comboProgram;
    QLabel *lblImage;
    
    QRadioButton *rdoImage, *rdoImageSlow, *rdoImageFPN;
    QRadioButton *rdoTrack, *rdoTrackEmbed, *rdoTrackTrajLinear, *rdoTrackTrajNonlinear, *rdoTrackRPM;
    QLabel *lblDataPoints, *lblDataPointsUnits, *lblPredictPoints, *lblPredictPointsUnits, *lblPredictInterval, *lblPredictIntervalUnits;
    QSpinBox *spinDataPoints, *spinPredictPoints, *spinPredictInterval;
    
    QCheckBox *chkCDS, *chkParallel, *chkRepeat, *chkDebug, *chkDummy, *chkStretch, *chkFlip, *chkTransform;
    
    QLabel *lblDesc;
    
    QLabel *lblVmax, *lblVmaxUnits;
    QDoubleSpinBox *spinVmax;
    
    QLabel *lblVmin, *lblVminUnits;
    QDoubleSpinBox *spinVmin;
    
    QLabel *lblVrst, *lblVrstUnits;
    QDoubleSpinBox *spinVrst;
    
    QLabel *lblVbias, *lblVbiasUnits;
    QDoubleSpinBox *spinVbias;
    
    QLabel *lblTrst, *lblTrstUnits;
    QSpinBox *spinTrst;
    
    QLabel *lblTint, *lblTintUnits;
    QSpinBox *spinTint;
    
    QLabel *lblTimeout, *lblTimeoutUnits;
    QSpinBox *spinTimeout;

    QLabel *lblThreshold;
    QSpinBox *spinThreshold;
    
    QLabel *lblFrames, *lblFramesUnits;
    QSpinBox *spinFrames;
    
    QLabel *lblFPS,	*lblFPSUnits;
    QSpinBox *spinFPS;

    
    QPushButton *btnApply, *btnReset, *btnStop, *btnStart;
    
    QPushButton *btnExportImage, *btnRecordStart, *btnRecordStop;
    QProgressBar *barRecord;
    
    QMenuBar *menubar;
    QMenu *menuFile, *menuTools;
    QStatusBar *statusbar;

    void setupUI(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        
        MainWindow->resize(1080, 725);
        MainWindow->setFixedSize(1080, 725);
        MainWindow->setDocumentMode(false);
        //MainWindow->statusBar()->setSizeGripEnabled(false);
        
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        
        frameVideo = new QFrame(centralwidget);
        frameVideo->setObjectName("frameVideo");
        frameVideo->setGeometry(QRect(15, 15, 800, 640));
        frameVideo->setFrameShape(QFrame::StyledPanel);
        frameVideo->setFrameShadow(QFrame::Raised);
        
        lcdFPS = new QLabel(MainWindow);
        lcdFPS->setObjectName("lcdFPS");
        lcdFPS->setGeometry(QRect(735, 685, 80, 35));
        QPalette lcdPal = lcdFPS->palette();
        lcdPal.setColor(lcdPal.WindowText, QColor(255,255,255));
        lcdPal.setColor(lcdPal.Background, QColor(  0,  0,  0));
        lcdFPS->setPalette(lcdPal);
        lcdFPS->setAutoFillBackground(false);
        lcdFPS->setAlignment(Qt::AlignRight);
        lcdFPS->setStyleSheet("font-family: Terminus; font-size: 12pt; font-weight: bold");
        
        tabConfig = new QTabWidget(centralwidget);
        tabConfig->setObjectName("tabConfig");
        tabConfig->setGeometry(QRect(830, 15, 235, 490));
        tabConfig->addTab(new QWidget(), "Program");
        tabConfig->addTab(new QWidget(), "Settings");
        
        grpImage = new QButtonGroup(tabConfig->widget(0));
        grpImage->setObjectName("grpImage");
        grpImage->setExclusive(true);
        
        grpTrack = new QButtonGroup(tabConfig->widget(0));
        grpTrack->setObjectName("grpTrack");
        grpTrack->setExclusive(true);
        
        grpFlags = new QButtonGroup(tabConfig->widget(0));
        grpFlags->setObjectName("grpFlags");
        grpFlags->setExclusive(false);
        
        tabActions = new QTabWidget(centralwidget);
        tabActions->setObjectName("tabActions");
        tabActions->setGeometry(QRect(830, 525, 235, 130));
        tabActions->addTab(new QWidget(), "Stream");
        tabActions->addTab(new QWidget(), "Record");
        
        
        
        layoutConfig = new QVBoxLayout(tabConfig->widget(0));
        layoutConfig->setObjectName("layoutConfig");
        
        layoutOpts = new QStackedLayout();
        layoutOpts->setObjectName("layoutOpts");
        
        layoutProgram = new QGridLayout();
        layoutProgram->setObjectName("layoutProgram");
        
        layoutBlank = new QGridLayout();
        layoutBlank->setObjectName("layoutBlank");
        
        layoutImage = new QGridLayout();
        layoutImage->setObjectName("layoutImage");
        
        layoutTrack = new QGridLayout();
        layoutTrack->setObjectName("layoutTrack");
        
        layoutFlags = new QGridLayout();
        layoutFlags->setObjectName("layoutFlags");
        layoutFlags->setVerticalSpacing(15);

        QWidget *widBlank = new QWidget();
        widBlank->setLayout(layoutBlank);
        QWidget *widImage = new QWidget();
        widImage->setLayout(layoutImage);
        QWidget *widTrack = new QWidget();
        widTrack->setLayout(layoutTrack);
        layoutOpts->addWidget(widBlank);
        layoutOpts->addWidget(widImage);
        layoutOpts->addWidget(widTrack);
        layoutConfig->addLayout(layoutProgram);
        layoutConfig->addLayout(layoutOpts);
        layoutConfig->addLayout(layoutFlags);
        
        layoutSettings = new QGridLayout(tabConfig->widget(1));
        layoutSettings->setObjectName("layoutSettings");
        
        layoutActions = new QGridLayout(tabActions->widget(0));
        layoutActions->setObjectName("layoutActions");
        
        layoutRecord = new QGridLayout(tabActions->widget(1));
        layoutRecord->setObjectName("layoutRecord");
        
        
        
        lblImage = new QLabel(frameVideo);
        lblImage->setObjectName("lblImage");
        lblImage->setGeometry(QRect(0, 0, 800, 640));
        lblImage->setAlignment(Qt::AlignCenter);

        
        
        lblProgram = new QLabel(tabConfig->widget(0));
        lblProgram->setObjectName("lblProgram");
        lblProgram->setText("Operation");
        comboProgram = new QComboBox(tabConfig->widget(0));
        comboProgram->setObjectName("comboProgram");
        lblProgram->setBuddy(comboProgram);

        layoutProgram->addWidget(lblProgram, 0, 0, Qt::AlignLeft);
        layoutProgram->addWidget(comboProgram, 0, 1, 1, 2);
        
        layoutProgram->setColumnStretch(0, 1);
        layoutProgram->setColumnStretch(1, 1);
        
        lblDesc = new QLabel(layoutOpts->widget(0));
        lblDesc->setGeometry(QRect(0, 10, 178, 90));
        lblDesc->setObjectName("lblDesc");
        lblDesc->setAlignment(Qt::AlignTop);
        lblDesc->setText("");
        lblDesc->setWordWrap(true);

        
        
        rdoImage = new QRadioButton(layoutOpts->widget(1));
        rdoImage->setObjectName("rdoImage");
        rdoImage->setText("Output | Integrate");

        rdoImageSlow = new QRadioButton(layoutOpts->widget(1));
        rdoImageSlow->setObjectName("rdoImageSlow");
        rdoImageSlow->setText("Integrate | Output");

        rdoImageFPN = new QRadioButton(layoutOpts->widget(1));
        rdoImageFPN->setObjectName("rdoImageFPN");
        rdoImageFPN->setText("Fixed Pattern Noise");

        grpImage->addButton(rdoImage);
        grpImage->addButton(rdoImageSlow);
        grpImage->addButton(rdoImageFPN);
        
        layoutImage->setVerticalSpacing(4);
        layoutImage->addWidget(rdoImage,     0, 0);
        layoutImage->addWidget(rdoImageSlow, 1, 0);
        layoutImage->addWidget(rdoImageFPN,  2, 0);

        
        
        //rdoTrack = new QRadioButton(layoutOpts->widget(2));
        rdoTrack = new QRadioButton();
        rdoTrack->setObjectName("rdoTrack");
        rdoTrack->setText("Data + Image Packets");

        rdoTrackEmbed = new QRadioButton(layoutOpts->widget(2));
        rdoTrackEmbed->setObjectName("rdoTrackEmbed");
        rdoTrackEmbed->setText("Embed Data in Image");

        rdoTrackTrajLinear = new QRadioButton(layoutOpts->widget(2));
        rdoTrackTrajLinear->setObjectName("rdoTrackTrajLinear");
        rdoTrackTrajLinear->setText("Linear Trajectory");

        rdoTrackTrajNonlinear = new QRadioButton(layoutOpts->widget(2));
        rdoTrackTrajNonlinear->setObjectName("rdoTrackTrajNonlinear");
        rdoTrackTrajNonlinear->setText("Nonlinear Trajectory");

        //rdoTrackRPM = new QRadioButton(layoutOpts->widget(2));
        rdoTrackRPM = new QRadioButton();
        rdoTrackRPM->setObjectName("rdoTrackRPM");
        rdoTrackRPM->setText("RPM (untested)");

        lblDataPoints = new QLabel(tabConfig->widget(0));
        lblDataPoints->setObjectName("lblDataPoints");
        lblDataPoints->setText("Draw");
        spinDataPoints = new QSpinBox(tabConfig->widget(0));
        spinDataPoints->setObjectName("spinDataPoints");
        spinDataPoints->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinDataPoints->setAccelerated(true);
        spinDataPoints->setMaximum(1024);
        spinDataPoints->setAlignment(Qt::AlignRight);
        lblDataPointsUnits = new QLabel(tabConfig->widget(0));
        lblDataPointsUnits->setObjectName("lblDataPointsUnits");
        lblDataPointsUnits->setText("points");
        lblDataPoints->setBuddy(spinDataPoints);

        lblPredictPoints = new QLabel(tabConfig->widget(0));
        lblPredictPoints->setObjectName("lblPredictPoints");
        lblPredictPoints->setText("Predict");
        spinPredictPoints = new QSpinBox(tabConfig->widget(0));
        spinPredictPoints->setObjectName("spinPredictPoints");
        spinPredictPoints->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinPredictPoints->setAccelerated(false);
        spinPredictPoints->setMaximum(6);
        spinPredictPoints->setAlignment(Qt::AlignRight);
        lblPredictPointsUnits = new QLabel(tabConfig->widget(0));
        lblPredictPointsUnits->setObjectName("lblPredictPointsUnits");
        lblPredictPointsUnits->setText("points");
        lblPredictPoints->setBuddy(spinPredictPoints);
        
        lblPredictInterval = new QLabel(tabConfig->widget(0));
        lblPredictInterval->setObjectName("lblPredictInterval");
        lblPredictInterval->setText("Every");
        spinPredictInterval = new QSpinBox(tabConfig->widget(0));
        spinPredictInterval->setObjectName("spinPredictInterval");
        spinPredictInterval->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinPredictInterval->setAccelerated(true);
        spinPredictInterval->setMaximum(1000);
        spinPredictInterval->setMinimum(1);
        spinPredictInterval->setAlignment(Qt::AlignRight);
        lblPredictIntervalUnits = new QLabel(tabConfig->widget(0));
        lblPredictIntervalUnits->setObjectName("lblPredictIntervalUnits");
        lblPredictIntervalUnits->setText("frames");
        lblPredictInterval->setBuddy(spinPredictInterval);
        
        grpTrack->addButton(rdoTrack);
        grpTrack->addButton(rdoTrackEmbed);
        grpTrack->addButton(rdoTrackTrajLinear);
        grpTrack->addButton(rdoTrackTrajNonlinear);
        grpTrack->addButton(rdoTrackRPM);
        
        layoutTrack->setVerticalSpacing(4);
        //layoutTrack->addWidget(rdoTrack,              0, 0, 1, 3); // i never use these ..
        //layoutTrack->addWidget(rdoTrackRPM,           2, 0, 1, 3);
        layoutTrack->addWidget(rdoTrackEmbed,           1, 0, 1, 3);
        layoutTrack->addWidget(rdoTrackTrajLinear,      2, 0, 1, 3);
        layoutTrack->addWidget(rdoTrackTrajNonlinear,   3, 0, 1, 3);
        layoutTrack->addWidget(lblDataPoints,           4, 0);
        layoutTrack->addWidget(spinDataPoints,          4, 1);
        layoutTrack->addWidget(lblDataPointsUnits,      4, 2);
        layoutTrack->addWidget(lblPredictPoints,        5, 0);
        layoutTrack->addWidget(spinPredictPoints,       5, 1);
        layoutTrack->addWidget(lblPredictPointsUnits,   5, 2);
        layoutTrack->addWidget(lblPredictInterval,      6, 0);
        layoutTrack->addWidget(spinPredictInterval,     6, 1);
        layoutTrack->addWidget(lblPredictIntervalUnits, 6, 2);



        chkCDS = new QCheckBox("Double Sampling", tabConfig->widget(0));
        chkCDS->setObjectName("chkCDS");

        chkParallel = new QCheckBox("Parallel Readout", tabConfig->widget(0));
        chkParallel->setObjectName("chkParallel");

        chkRepeat = new QCheckBox("Repeat Record", tabConfig->widget(0));
        chkRepeat->setObjectName("chkRepeat");

        chkDebug = new QCheckBox("Console Output", tabConfig->widget(0));
        chkDebug->setObjectName("chkDebug");

        chkDummy = new QCheckBox("Random Data", tabConfig->widget(0));
        chkDummy->setObjectName("chkDummy");

        chkStretch = new QCheckBox("Stretch", tabConfig->widget(0));
        chkStretch->setObjectName("chkStretch");

        chkFlip = new QCheckBox("Vertical Flip", tabConfig->widget(0));
        chkFlip->setObjectName("chkFlip");

        chkTransform = new QCheckBox("Transform Readout", tabConfig->widget(0));
        chkTransform->setObjectName("chkTransform");

        grpFlags->addButton(chkCDS);
        grpFlags->addButton(chkParallel);
        grpFlags->addButton(chkRepeat);
        grpFlags->addButton(chkDebug);
        grpFlags->addButton(chkDummy);
        grpFlags->addButton(chkStretch);
        grpFlags->addButton(chkFlip);
        grpFlags->addButton(chkTransform);
        
        layoutFlags->addWidget(chkCDS,       0, 0);
        layoutFlags->addWidget(chkParallel,  1, 0);
        layoutFlags->addWidget(chkRepeat,    2, 0);
        layoutFlags->addWidget(chkDebug,     3, 0);
        layoutFlags->addWidget(chkDummy,     4, 0);
        layoutFlags->addWidget(chkStretch,   5, 0);
        layoutFlags->addWidget(chkFlip,      6, 0);
        layoutFlags->addWidget(chkTransform, 7, 0);



        lblVmax = new QLabel(tabConfig->widget(1));
        lblVmax->setObjectName("lblVmax");
        lblVmax->setText(QApplication::translate("MainWindow", "Vmax", 0));
        lblVmaxUnits = new QLabel(tabConfig->widget(1));
        lblVmaxUnits->setObjectName("lblVmaxUnits");
        lblVmaxUnits->setText(QApplication::translate("MainWindow", "V", 0));
        spinVmax = new QDoubleSpinBox(tabConfig->widget(1));
        spinVmax->setObjectName("spinVmax");
        spinVmax->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinVmax->setAccelerated(true);
        spinVmax->setMaximum(2.5);
        spinVmax->setSingleStep(0.05);
        spinVmax->setAlignment(Qt::AlignRight);
        lblVmax->setBuddy(spinVmax);

        lblVmin = new QLabel(tabConfig->widget(1));
        lblVmin->setObjectName("lblVmin");
        lblVmin->setText(QApplication::translate("MainWindow", "Vmin", 0));
        lblVminUnits = new QLabel(tabConfig->widget(1));
        lblVminUnits->setObjectName("lblVminUnits");
        lblVminUnits->setText(QApplication::translate("MainWindow", "V", 0));
        spinVmin = new QDoubleSpinBox(tabConfig->widget(1));
        spinVmin->setObjectName("spinVmin");
        spinVmin->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinVmin->setAccelerated(true);
        spinVmin->setMaximum(2.5);
        spinVmin->setSingleStep(0.05);
        spinVmin->setAlignment(Qt::AlignRight);
        lblVmax->setBuddy(spinVmax);
        lblVmin->setBuddy(spinVmin);

        lblVrst = new QLabel(tabConfig->widget(1));
        lblVrst->setObjectName("lblVrst");
        lblVrst->setText(QApplication::translate("MainWindow", "Vrst", 0));
        lblVrstUnits = new QLabel(tabConfig->widget(1));
        lblVrstUnits->setObjectName("lblVrstUnits");
        lblVrstUnits->setText(QApplication::translate("MainWindow", "V", 0));
        spinVrst = new QDoubleSpinBox(tabConfig->widget(1));
        spinVrst->setObjectName("spinVrst");
        spinVrst->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinVrst->setAccelerated(true);
        spinVrst->setMaximum(2.5);
        spinVrst->setSingleStep(0.05);
        spinVrst->setAlignment(Qt::AlignRight);
        lblVrst->setBuddy(spinVrst);

        lblVbias = new QLabel(tabConfig->widget(1));
        lblVbias->setObjectName("lblVbias");
        lblVbias->setText(QApplication::translate("MainWindow", "Vbias", 0));
        lblVbiasUnits = new QLabel(tabConfig->widget(1));
        lblVbiasUnits->setObjectName("lblVbiasUnits");
        lblVbiasUnits->setText(QApplication::translate("MainWindow", "V", 0));
        spinVbias = new QDoubleSpinBox(tabConfig->widget(1));
        spinVbias->setObjectName("spinVbias");
        spinVbias->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinVbias->setAccelerated(true);
        spinVbias->setMaximum(2.5);
        spinVbias->setSingleStep(0.005);
        spinVbias->setAlignment(Qt::AlignRight);
        lblVbias->setBuddy(spinVbias);
        
        lblTrst = new QLabel(tabConfig->widget(1));
        lblTrst->setObjectName("lblTrst");
        lblTrst->setText("Trst");
        lblTrstUnits = new QLabel(tabConfig->widget(1));
        lblTrstUnits->setObjectName("lblTrstUnits");
        lblTrstUnits->setText(mu + "s");
        spinTrst = new QSpinBox(tabConfig->widget(1));
        spinTrst->setObjectName("spinTrst");
        spinTrst->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinTrst->setAccelerated(true);
        spinTrst->setMaximum(1000000);
        spinTrst->setAlignment(Qt::AlignRight);
        lblTrst->setBuddy(spinTrst);

        lblTint = new QLabel(tabConfig->widget(1));
        lblTint->setObjectName("lblTint");
        lblTint->setText("Tint");
        lblTintUnits = new QLabel(tabConfig->widget(1));
        lblTintUnits->setObjectName("lblTintUnits");
        lblTintUnits->setText(mu + "s");
        spinTint = new QSpinBox(tabConfig->widget(1));
        spinTint->setObjectName("spinTint");
        spinTint->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinTint->setAccelerated(true);
        spinTint->setMaximum(1000000);
        spinTint->setAlignment(Qt::AlignRight);
        lblTint->setBuddy(spinTint);

        lblTimeout = new QLabel(tabConfig->widget(1));
        lblTimeout->setObjectName("lblTimeout");
        lblTimeout->setText(QApplication::translate("MainWindow", "Timeout", 0));
        lblTimeoutUnits = new QLabel(tabConfig->widget(1));
        lblTimeoutUnits->setObjectName("lblTimeoutUnits");
        lblTimeoutUnits->setText(QApplication::translate("MainWindow", "ms", 0));
        spinTimeout = new QSpinBox(tabConfig->widget(1));
        spinTimeout->setObjectName("spinTimeout");
        spinTimeout->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinTimeout->setAccelerated(true);
        spinTimeout->setMaximum(500);
        spinTimeout->setAlignment(Qt::AlignRight);
        lblTimeout->setBuddy(spinTimeout);

        lblThreshold = new QLabel(tabConfig->widget(1));
        lblThreshold->setObjectName("lblThreshold");
        lblThreshold->setText(QApplication::translate("MainWindow", "Threshold", 0));
        spinThreshold = new QSpinBox(tabConfig->widget(1));
        spinThreshold->setObjectName("spinThreshold");
        spinThreshold->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinThreshold->setAccelerated(true);
        spinThreshold->setMaximum(255);
        spinThreshold->setAlignment(Qt::AlignRight);

        lblFrames = new QLabel(tabActions->widget(1));
        lblFrames->setObjectName("lblFrames");
        lblFrames->setText(QApplication::translate("MainWindow", "Capture", 0));
        lblFramesUnits = new QLabel(tabActions->widget(1));
        lblFramesUnits->setObjectName("lblFramesUnits");
        lblFramesUnits->setText(QApplication::translate("MainWindow", "frames", 0));
        spinFrames = new QSpinBox(tabActions->widget(1));
        spinFrames->setObjectName("spinFrames");
        spinFrames->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinFrames->setAccelerated(true);
        spinFrames->setMaximum(1000000);
        spinFrames->setAlignment(Qt::AlignRight);
        lblFrames->setBuddy(spinFrames);
        
        lblFPS = new QLabel(tabActions->widget(1));
        lblFPS->setObjectName("lblFPS");
        lblFPS->setText(QApplication::translate("MainWindow", "Encode", 0));
        lblFPSUnits = new QLabel(tabActions->widget(1));
        lblFPSUnits->setObjectName("lblFPSUnits");
        lblFPSUnits->setText(QApplication::translate("MainWindow", "fps", 0));
        spinFPS = new QSpinBox(tabActions->widget(1));
        spinFPS->setObjectName("spinFPS");
        spinFPS->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        spinFPS->setAccelerated(true);
        spinFPS->setMaximum(2000);
        spinFPS->setValue(30);
        spinFPS->setAlignment(Qt::AlignRight);
        lblFPS->setBuddy(spinFPS);

        layoutSettings->addWidget(lblVmax,         0, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinVmax,        0, 1);
        layoutSettings->addWidget(lblVmaxUnits,    0, 2);
        
        layoutSettings->addWidget(lblVmin,         1, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinVmin,        1, 1);
        layoutSettings->addWidget(lblVminUnits,    1, 2);
        
        layoutSettings->addWidget(lblVrst,         2, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinVrst,        2, 1);
        layoutSettings->addWidget(lblVrstUnits,    2, 2);
        
        layoutSettings->addWidget(lblVbias,        3, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinVbias,       3, 1);
        layoutSettings->addWidget(lblVbiasUnits,   3, 2);
        
        layoutSettings->addWidget(lblTrst,         4, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinTrst,        4, 1);
        layoutSettings->addWidget(lblTrstUnits,    4, 2);
        
        layoutSettings->addWidget(lblTint,         5, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinTint,        5, 1);
        layoutSettings->addWidget(lblTintUnits,    5, 2);
        
        layoutSettings->addWidget(lblTimeout,      6, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinTimeout,     6, 1);
        layoutSettings->addWidget(lblTimeoutUnits, 6, 2);
        
        layoutSettings->addWidget(lblThreshold,    7, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinThreshold,   7, 1);
        
        layoutSettings->addWidget(lblFrames,       8, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinFrames,      8, 1);
        layoutSettings->addWidget(lblFramesUnits,  8, 2);
        
        layoutSettings->addWidget(lblFPS,          9, 0, Qt::AlignLeft);
        layoutSettings->addWidget(spinFPS,         9, 1);
        layoutSettings->addWidget(lblFPSUnits,     9, 2);

        layoutSettings->setColumnStretch(0, 2);
        layoutSettings->setColumnStretch(1, 4);
        layoutSettings->setColumnStretch(2, 1);



        
        btnStart = new QPushButton(tabActions->widget(0));
        btnStart->setObjectName("btnStart");
        btnStart->setText(QApplication::translate("MainWindow", "&Start", 0));
        //btnStart->setText(iconPlay);

        btnStop = new QPushButton(tabActions->widget(0));
        btnStop->setObjectName("btnStop");
        btnStop->setText(QApplication::translate("MainWindow", "St&op", 0));
        //btnStop->setText(iconStop);

        btnApply = new QPushButton(tabActions->widget(0));
        btnApply->setObjectName("btnApply");
        btnApply->setText(QApplication::translate("MainWindow", "&Apply", 0));
        //btnApply->setText(iconUpload);

        btnReset = new QPushButton(tabActions->widget(0));
        btnReset->setObjectName("btnReset");
        btnReset->setText(QApplication::translate("MainWindow", "&Reset", 0));
        //btnReset->setText(iconReset);

        layoutActions->addWidget(btnApply, 0, 0);
        layoutActions->addWidget(btnStart, 0, 1);
        layoutActions->addWidget(btnReset, 1, 0);
        layoutActions->addWidget(btnStop,  1, 1);

        
        
        btnRecordStart = new QPushButton(tabActions->widget(1));
        btnRecordStart->setObjectName("btnRecordStart");
        btnRecordStart->setText(QApplication::translate("MainWindow", "Start", 0));
        //btnRecordStart->setText(iconRecord);

        btnRecordStop = new QPushButton(tabActions->widget(1));
        btnRecordStop->setObjectName("btnRecordStop");
        btnRecordStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        //btnRecordStop->setText(iconPause);

        btnExportImage = new QPushButton(tabActions->widget(1));
        btnExportImage->setObjectName("btnExportImage");
        btnExportImage->setText(QApplication::translate("MainWindow", "Image", 0));
        //btnExportImage->setText(iconSave);
        
		barRecord = new QProgressBar(tabActions->widget(1));
		barRecord->setFormat("%v/%m");
		barRecord->setMinimum(0);
		barRecord->setMaximum(1);
		barRecord->setValue(0);
		barRecord->reset();

        layoutRecord->addWidget(btnRecordStart, 0, 0, 1, 1);
        layoutRecord->addWidget(btnRecordStop,  0, 1, 1, 1);
        layoutRecord->addWidget(btnExportImage, 0, 2, 1, 1);
        layoutRecord->addWidget(barRecord,      1, 0, 1, 3);

        
        
        MainWindow->setCentralWidget(centralwidget);

        // ??
        //lcdFPS->raise();
        //tabActions->widget(1)->raise();
        
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 725, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuTools = new QMenu(menubar);
        menuTools->setObjectName("menuTools");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        
        actionRecord = new QAction(MainWindow);
        actionRecord->setObjectName("actionRecord");
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName("actionExport");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName("actionSetup");
        actionSetup->setShortcut(Qt::CTRL + Qt::Key_M);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuFile->addAction(actionRecord);
        menuFile->addSeparator();
        menuFile->addAction(actionExport);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTools->addAction(actionSetup);
        

        QWidget::setTabOrder(comboProgram, chkCDS);
        QWidget::setTabOrder(chkCDS, chkParallel);
        QWidget::setTabOrder(chkParallel, chkRepeat);
        QWidget::setTabOrder(chkRepeat, chkDebug);
        QWidget::setTabOrder(chkDebug, chkDummy);
        QWidget::setTabOrder(chkDummy, chkStretch);
        QWidget::setTabOrder(chkStretch, chkFlip);
        QWidget::setTabOrder(chkFlip, chkTransform);
        QWidget::setTabOrder(chkTransform, btnApply);
        
        QWidget::setTabOrder(btnApply, btnStart);
        QWidget::setTabOrder(btnStart, btnStop);
        QWidget::setTabOrder(btnStop,  btnReset);

        QWidget::setTabOrder(spinVmax, spinVmin);
        QWidget::setTabOrder(spinVmin, spinVrst);
        QWidget::setTabOrder(spinVrst, spinVbias);
        QWidget::setTabOrder(spinVbias, spinTrst);
        QWidget::setTabOrder(spinTrst, spinTint);
        QWidget::setTabOrder(spinTint, spinTimeout);
        QWidget::setTabOrder(spinTimeout, spinThreshold);
        QWidget::setTabOrder(spinThreshold, spinFrames);
        QWidget::setTabOrder(spinFrames, spinFPS);
        QWidget::setTabOrder(spinFPS, btnRecordStart);

        QWidget::setTabOrder(btnRecordStart, btnRecordStop);
        QWidget::setTabOrder(btnRecordStop, btnExportImage);
        
        retranslateUI(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUI(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PIRANHA", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionRecord->setText(QApplication::translate("MainWindow", "Record", 0));
        actionExport->setText(QApplication::translate("MainWindow", "Export", 0));
        actionSetup->setText(QApplication::translate("MainWindow", "Setup", 0));
        lblImage->setText(QApplication::translate("MainWindow", "", 0));
        comboProgram->clear();
        comboProgram->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Imaging", 0)
         << QApplication::translate("MainWindow", "RCR Readout", 0)
         << QApplication::translate("MainWindow", "Edge Detect", 0)
         << QApplication::translate("MainWindow", "Median Filter", 0)
         << QApplication::translate("MainWindow", "DCT Filter", 0)
         << QApplication::translate("MainWindow", "Histogram", 0)
         << QApplication::translate("MainWindow", "Tracking", 0)
         << QApplication::translate("MainWindow", "AER", 0)
         << QApplication::translate("MainWindow", "Benchmark", 0)
        );
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0));
    } // retranslateUi

private slots:
	
	//void open();
	//void save();
	
	void export_image();
	void record_start();
	void record_stop();
	void exit();

	void setup();

	void apply();
	void start();
	void stop();
	void reset();
	
	void initValues();

	void imageChanged(int imageIndex);
	void trackChanged(int trackIndex);
	void flagsChanged(int flagsIndex);
	void programChanged(int programIndex);
	
	void printFrame();
	int acquireImage(cv::Mat*);
	QImage mat2qimage(const cv::Mat&);
	void trackDataPlot(cv::Mat*, cv::Mat*);
	void trackDataPlotEmbedded(cv::Mat*);
	void trackDataPlotTrajectory(cv::Mat*);

signals:
	void abortCapture();

};

struct dataCoor
{
	int x,y;
	dataCoor(int xx, int yy) : x(xx), y(yy) {}
};

QT_END_NAMESPACE

#endif // EXAMPLEPLI892_H
