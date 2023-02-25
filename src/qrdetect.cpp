#include "util.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// [[Rcpp::export]]
Rcpp::RObject cvmat_qrtext(XPtrMat ptr){
  QRCodeDetector qrDet = QRCodeDetector();
  std::string data = qrDet.detectAndDecode(get_mat(ptr));
  if(data.length()>0){
    return Rcpp::CharacterVector::create(data);
  }
  return R_NilValue;
}

// [[Rcpp::export]]
XPtrMat cvmat_qrmask(XPtrMat ptr){
  QRCodeDetector qrDet = QRCodeDetector();
  Mat img = get_mat(ptr);
  std::vector<Point> points;
  std::string data = qrDet.detectAndDecode(img, points);
  if (data.length() && !points.empty()) {
    for (int i = 0; i < points.size(); i++) {
      Point pt1 = points[i];
      Point pt2 = points[(i + 1) % 4];
      line(img, pt1, pt2, Scalar(255, 0, 0), 3);
    }
    cv::putText(img, data,
                cv::Point(10, img.rows / 2), //top-left position
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                CV_RGB(118, 185, 0), //font color
                2);

  }
  return ptr;
}

