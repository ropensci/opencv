#include "util.hpp"

// [[Rcpp::export]]
XPtrMat cvmat_rect(XPtrMat ptr, int x = 0, int y = 0, int width = 0, int height = 0){
  cv::Mat img = get_mat(ptr);
  cv::Rect roi = cv::Rect(x, y, width, height);
  cv::Mat output = img(roi);
  return cvmat_xptr(output);
}
