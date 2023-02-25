#include "util.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// [[Rcpp::export]]
Rcpp::RObject cvmat_qrcode(XPtrMat ptr){
  QRCodeDetector qrDet = QRCodeDetector();
  std::string data = qrDet.detectAndDecode(get_mat(ptr));
  if(data.length()>0){
    return Rcpp::CharacterVector::create(data);
  }
  return R_NilValue;
}
