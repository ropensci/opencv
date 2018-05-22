#define R_NO_REMAP
#define STRICT_R_HEADERS

#include <Rcpp.h>
#include <opencv2/opencv.hpp>

void finalize_mat(cv::Mat * frame);
typedef Rcpp::XPtr<cv::Mat, Rcpp::PreserveStorage, finalize_mat, true> XPtrMat;
