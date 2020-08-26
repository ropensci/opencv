#include "util.hpp"

// [[Rcpp::export]]
XPtrMat cvmat_grayscale(XPtrMat src){
  cv::Mat output;
  cv::cvtColor(get_mat(src), output, cv::COLOR_BGR2GRAY);
  return cvmat_xptr(output);
}
