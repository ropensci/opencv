#include "util.hpp"

using namespace cv;
using namespace std;

// [[Rcpp::export]]
XPtrMat cvmat_morphology(XPtrMat input, int operation, int k_shape, int k_height, int k_width, int iterations) {
  cv::Mat out;
  cv::Mat k = cv::getStructuringElement(k_shape, cv::Size(2 * k_width + 1, 2 * k_height + 1));
  cv::morphologyEx(get_mat(input), out, operation, k, cv::Point(-1, -1), iterations);
  return cvmat_xptr(out);
}

// [[Rcpp::export]]
XPtrMat cvmat_morphology_custom(XPtrMat input, int operation, Rcpp::NumericVector kernel, int iterations) {
  cv::Mat out, k;
  Rcpp::IntegerVector kernelDims = kernel.attr("dim");
  k.create(kernelDims[0], kernelDims[1], CV_8U);

  if (operation != 7) {
    for(int i = 0; i < kernelDims[0]; i++) {
      for(int j = 0; j < kernelDims[1]; j++) {
        k.at<unsigned char>(i, j) = kernel[kernelDims[0] * j + i];
      }
    }
  } else {
    for(int i = 0; i < kernelDims[0]; i++) {
      for(int j = 0; j < kernelDims[1]; j++) {
        k.at<int>(i, j) = kernel[kernelDims[0] * j + i];
      }
    }
  }

  cv::morphologyEx(get_mat(input), out, operation, k, cv::Point(-1, -1), iterations);
  return cvmat_xptr(out);
}
