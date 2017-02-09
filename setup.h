#include <QtGui>
#include <iostream>
//#include <QtCore/QProcess>

QT_BEGIN_NAMESPACE

class UIsetup : public QMainWindow
{

Q_OBJECT

public:
    UIsetup();
    ~UIsetup();

    void Device(std::string*);
    void Debug(bool);

private:

	void deviceScan();

	QProcess *procSetup;
	
    QString iconPlay	= QChar(0x23f5);
    QString iconPause	= QChar(0x23f8);
    QString iconStop	= QChar(0x23f9);
    QString iconRecord	= QChar(0x23fa);
    QString iconUpload	= QChar(0x23f6);
    QString iconReset	= QChar(0x2b6f);
    QString iconSave	= QChar(0x1f4be);

    QWidget *centralwidget;

    QHBoxLayout *layoutMain;
    
    QTextEdit *txtConsole;

    QComboBox *comboDevice;
    
    QVBoxLayout *layoutSetup;

    QGridLayout *layoutGrid;
    
    QLabel *lblProgram;
    
    QPushButton *btnScan, *btnModule, *btnPlatform, *btnJTAG, *btnUSB, *btnClose;
    
    QButtonGroup *grpSetup;
    
    QProgressBar *barRecord;
    
    //QStatusBar *statusbar;

    void setupUI(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        
        MainWindow->resize(700, 400);
        MainWindow->setFixedSize(700, 400);
        MainWindow->setDocumentMode(false);
        
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        
        //statusbar = new QStatusBar(centralwidget);
        //statusbar->setObjectName("statusbar");
        //MainWindow->setStatusBar(statusbar);

        layoutMain = new QHBoxLayout(centralwidget);
        layoutMain->setObjectName("layoutMain");
        
        txtConsole = new QTextEdit(centralwidget);
        txtConsole->setObjectName("txtConsole");
        //txtConsole->setGeometry(5, 5, 250, 285);
        txtConsole->setReadOnly(true);
        //txtConsole->setStyleSheet("font-family: Terminus; font-size: 9pt; font-weight: regular");
        txtConsole->setStyleSheet("font-family: Source Code Pro; font-size: 10pt; font-weight: regular");

        grpSetup = new QButtonGroup(centralwidget);
        grpSetup->setObjectName("grpSetup");
        grpSetup->setExclusive(false);
        
        layoutSetup = new QVBoxLayout();
        layoutSetup->setObjectName("layoutSetup");
        //layoutSetup->setGeometry(255, 5, 250, 285);
        layoutGrid = new QGridLayout();
        layoutGrid->setObjectName("layoutGrid");
        layoutSetup->addLayout(layoutGrid);

        comboDevice = new QComboBox(centralwidget);
        comboDevice->setObjectName("comboDevice");
        comboDevice->clear();
        //comboDevice->insertItems(0, QStringList() << "UM232H");
        
        btnScan = new QPushButton(centralwidget);
        btnScan->setObjectName("btnScan");
        btnScan->setText("Scan");
        grpSetup->addButton(btnScan);

        btnModule = new QPushButton(centralwidget);
        btnModule->setObjectName("btnModule");
        btnModule->setText("Unload ftdi_sio");
        grpSetup->addButton(btnModule);

        btnPlatform = new QPushButton(centralwidget);
        btnPlatform->setObjectName("btnPlatform");
        btnPlatform->setText("Platform Cable");
        grpSetup->addButton(btnPlatform);

        btnJTAG = new QPushButton(centralwidget);
        btnJTAG->setObjectName("btnJTAG");
        btnJTAG->setText("JTAG Program");
        grpSetup->addButton(btnJTAG);

        btnUSB = new QPushButton(centralwidget);
        btnUSB->setObjectName("btnUSB");
        btnUSB->setText("USB Program");
        btnUSB->setEnabled(false);
        grpSetup->addButton(btnUSB);

        btnClose = new QPushButton(centralwidget);
        btnClose->setObjectName("btnClose");
        btnClose->setText("Close");
        grpSetup->addButton(btnClose);

        layoutMain->addWidget(txtConsole);
        layoutMain->addLayout(layoutSetup);
        layoutMain->setStretch(0, 5);
        layoutMain->setStretch(1, 1);
        
        layoutGrid->addWidget(btnModule,   0, 0);
        layoutGrid->addWidget(btnScan,     1, 0);
        layoutGrid->addWidget(comboDevice, 2, 0);
        layoutGrid->addWidget(btnPlatform, 3, 0);
        layoutGrid->addWidget(btnJTAG,     4, 0);
        layoutGrid->addWidget(btnUSB,      5, 0);
        layoutGrid->addWidget(btnClose,    6, 0);

        retranslateUI(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUI(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Setup", 0));
    } // retranslateUi

private slots:
	
	void scan();
	void module();
	void programPlatform();
	void programJTAG();
	void programUSB();
	void procStdErr();
	void procStdOut();
	void deviceChanged(QString);

};

QT_END_NAMESPACE
