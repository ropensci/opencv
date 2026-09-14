#define R_NO_REMAP
#define STRICT_R_HEADERS

#include <Rcpp.h>
#include <opencv2/opencv.hpp>

/* OpenCV 5 moved boundingRect/convexHull into the new 'geometry' module and
   CascadeClassifier into the 'xobjdetect' contrib module. */
#if CV_VERSION_MAJOR >= 5
#ifndef HAVE_OPENCV_XOBJDETECT
#error "This package requires an OpenCV 5 build that includes the contrib 'xobjdetect' module"
#endif
#include <opencv2/geometry.hpp>
#include <opencv2/xobjdetect.hpp>
#endif

#if (CV_VERSION_MAJOR * 100 + CV_VERSION_MINOR * 10 + CV_VERSION_REVISION >= 452)
#define HAVE_WECHATQR
#endif

void finalize_mat(cv::Mat * frame);
typedef Rcpp::XPtr<cv::Mat, Rcpp::PreserveStorage, finalize_mat, true> XPtrMat;
