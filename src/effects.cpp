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
#if CV_VERSION_EPOCH < 3
  throw std::runtime_error("pencilSketch requires OpenCV 3 or newer");
#else
  pencilSketch(get_mat(ptr),out1, out2, 10 , 0.1f, 0.03f);
#endif
  return cvmat_xptr(color ? out2 : out1);
}

// [[Rcpp::export]]
XPtrMat cvmat_stylize(XPtrMat ptr){
  Mat out;
#if CV_VERSION_EPOCH < 3
  throw std::runtime_error("stylization requires OpenCV 3 or newer");
#else
  stylization(get_mat(ptr), out);
#endif
  return cvmat_xptr(out);
}
