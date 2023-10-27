#include "util.hpp"
#include <opencv2/wechat_qrcode.hpp>

using namespace cv;

std::string qr_scan_opencv(XPtrMat ptr){
  static QRCodeDetector qrDet = QRCodeDetector();
  return qrDet.detectAndDecode(get_mat(ptr));
}

std::string qr_scan_wechat(XPtrMat ptr){
  static wechat_qrcode::WeChatQRCode qrDet = wechat_qrcode::WeChatQRCode();
  std::vector<std::string> data = qrDet.detectAndDecode(get_mat(ptr));
  if(data.size() > 0)
    return data.at(0);
  return std::string();
}

// [[Rcpp::export]]
Rcpp::RObject cvmat_qrtext(XPtrMat ptr, bool use_wechat = true){
  std::string data = use_wechat ? qr_scan_wechat(ptr) : qr_scan_opencv(ptr);
  if(data.length()){
    return Rcpp::CharacterVector::create(data);
  }
  return R_NilValue;
}

// [[Rcpp::export]]
XPtrMat cvmat_qrmask(XPtrMat ptr){
  static QRCodeDetector qrDet = QRCodeDetector();
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
    ptr.attr("value") = Rcpp::CharacterVector::create(data);
  }
  return ptr;
}

