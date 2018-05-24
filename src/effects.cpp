#include "util.hpp"

using namespace cv;

// [[Rcpp::export]]
XPtrMat cvmat_blur(XPtrMat ptr, size_t ksize){
  Mat out;
  cv::blur(get_mat(ptr), out, Size(ksize, ksize));
  return cvmat_xptr(out);
}

// [[Rcpp::export]]
XPtrMat cvmat_sketch(XPtrMat ptr, bool color){
  Mat out1;
  Mat out2;
  pencilSketch(get_mat(ptr),out1, out2, 10 , 0.1f, 0.03f);
  return cvmat_xptr(color ? out2 : out1);
}

// [[Rcpp::export]]
XPtrMat cvmat_stylize(XPtrMat ptr){
  Mat out;
  stylization(get_mat(ptr), out);
  return cvmat_xptr(out);
}
