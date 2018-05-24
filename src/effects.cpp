#include "util.hpp"

using namespace cv;

// [[Rcpp::export]]
XPtrMat cvmat_blur(XPtrMat ptr, size_t ksize){
  Mat out;
  cv::blur(get_mat(ptr), out, Size(ksize, ksize));
  return cvmat_xptr(out);
}
