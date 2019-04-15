#include <Rcpp.h>
#include "util.hpp"
#include <opencv2/imgproc.hpp>

using namespace Rcpp;
using namespace cv;

// [[Rcpp::export]]
XPtrMat cv_dist_transform(XPtrMat src, int	distanceType,
                               int maskSize, int dstType){
  cv::Mat out;
  cv::distanceTransform(get_mat(src), out, distanceType, maskSize, dstType);
  return cvmat_xptr(out);
}


// [[Rcpp::export]]
XPtrMat cv_grayscale(XPtrMat src){
  cv::Mat greyMat;
  cv::cvtColor(get_mat(src), greyMat, cv::COLOR_BGR2GRAY);
  return cvmat_xptr(greyMat);
}

// [[Rcpp::export]]
XPtrMat cv_binary(XPtrMat src){
  cv::Mat binMat;
  cv::threshold(get_mat(src), binMat, 100,255, THRESH_BINARY);
  return cvmat_xptr(binMat);
}


