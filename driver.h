#include <ftd2xx.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Driver {

   FT_HANDLE ftHandle;
   FT_STATUS ftStatus;
   std::string driverstr;
   DWORD BytesReceived, BytesRecovered, BytesWritten;
   unsigned int imageSize;
   uint8_t bufferRx[5120];
   uint8_t bufferRecover[10240]; // be generous
   bool imgOK = false;
   
   const static bool debug = false;

public: 

   Driver(); 
   int connect(char* serial, uint timeoutms);
   int disconnect();
   int write(uint8_t data[], uint length);
   int captureRandom(cv::Mat*);
   int capture(uint8_t txBytes[], uint length, cv::Mat* img, bool transformimg);
   int captureData(uint8_t[], uint, uint8_t[], uint, cv::Mat*, cv::Mat*);
   int captureHistogram(uint8_t[], uint, uint8_t[], uint, cv::Mat*, cv::Mat*, cv::Mat*);
   int repeatCommand(uint8_t[], uint);
   int repeatData(cv::Mat*);
   cv::Mat fixImage(cv::Mat input);
};
