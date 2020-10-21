#define R_NO_REMAP
#define STRICT_R_HEADERS

#include <Rcpp.h>
#include <opencv2/opencv.hpp>

#if CV_VERSION_EPOCH >= 3 || (!defined(CV_VERSION_EPOCH) && CV_VERSION_MAJOR >= 3)
#define HAVE_OPENCV_3
#endif

#if !defined(CV_VERSION_EPOCH) && CV_VERSION_MAJOR >= 4
#define HAVE_OPENCV_4
#endif

void finalize_mat(cv::Mat * frame);
typedef Rcpp::XPtr<cv::Mat, Rcpp::PreserveStorage, finalize_mat, true> XPtrMat;
