#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <iostream>

#include "opencv2/imgproc.hpp"
using namespace std;
//using namespace cv;


class T
{
public:
  cv::Mat src, dst;
  int lowThreshold = 0;
  const int max_lowThreshold = 100;
  const int Ratio = 3;
  const int kernel_size = 3;
  const char* window_name = "Edge Map";


  int windowC;
  int windowW;
  double windowCenter, windowWidth;
  DicomImage* Dpointer;
};


static void bar(int windowC, void* Pointer)
{
  T* Tpointer = static_cast<T*>(Pointer);
  Tpointer->Dpointer->setWindow(static_cast<double>(windowC), Tpointer->windowWidth);
  Tpointer->src = cv::Mat(int(Tpointer->Dpointer->getWidth()), int(Tpointer->Dpointer->getHeight()), CV_8UC1, (Uint8*)Tpointer->Dpointer->getOutputData(8));

  Tpointer->windowCenter = static_cast<double>(windowC);
  imshow(Tpointer->window_name, Tpointer->src);
}

static void bar1(int windowW, void* Pointer)
{
  T* Tpointer = static_cast<T*>(Pointer);
  Tpointer->Dpointer->setWindow(Tpointer->windowCenter, static_cast<double>(windowW));
  Tpointer->src = cv::Mat(int(Tpointer->Dpointer->getWidth()), int(Tpointer->Dpointer->getHeight()), CV_8UC1, (Uint8*)Tpointer->Dpointer->getOutputData(8));

  Tpointer->windowWidth = static_cast<double>(windowW);
  imshow(Tpointer->window_name, Tpointer->src);
}



int main()
{
  DcmFileFormat file;
  std::string dicomFile = "E:\\Learning\\Data\\DICOM\\eclipse\\imrt\\CT.RT001921_4.dcm";
  OFCondition status = file.loadFile(dicomFile.c_str());
  if (status.bad())
  {
    cout << "error, file is not loaded." << endl;

  }
  DcmDataset* dataset = file.getDataset();
  E_TransferSyntax xfer = dataset->getOriginalXfer();

  /*T t;
  T* p = &t;*/
  T* p = new T;
  dataset->findAndGetFloat64(DCM_WindowCenter, p->windowCenter);
  dataset->findAndGetFloat64(DCM_WindowWidth, p->windowWidth);

  //constructor through dataset 
  DicomImage DCM_image(dataset, xfer);
  p->Dpointer = &DCM_image;

  p->dst.create(p->src.size(), p->src.type());
  cv::namedWindow(p->window_name, cv::WINDOW_AUTOSIZE);
  p->windowC = p->windowCenter;
  p->windowW = p->windowWidth;
  cv::createTrackbar("centerTrack", p->window_name, &p->windowC, 6000, bar, p);
  bar(p->windowCenter, p);

  cv::createTrackbar("widthTrack", p->window_name, &p->windowW, 6000, bar1, p);
  bar1(p->windowWidth, p);

  cv::waitKey();
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}