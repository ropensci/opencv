#include "util.hpp"
#include <opencv2/wechat_qrcode.hpp>

using namespace cv;

std::string qr_scan_opencv(XPtrMat ptr, Mat &points){
  static QRCodeDetector qrDet = QRCodeDetector();
  return qrDet.detectAndDecode(get_mat(ptr), points);
}

std::string qr_scan_wechat(XPtrMat ptr, Mat &points){
  static wechat_qrcode::WeChatQRCode qrDet = wechat_qrcode::WeChatQRCode();
  std::vector<Mat> pointsvec;
  std::vector<std::string> data = qrDet.detectAndDecode(get_mat(ptr), pointsvec);
  if(data.size() > 0){
    points = pointsvec.at(0);
    return data.at(0);
  }
  return std::string();
}

// [[Rcpp::export]]
Rcpp::RObject cvmat_qrtext(XPtrMat ptr, bool use_wechat = true){
  Mat points;
  std::string data = use_wechat ? qr_scan_wechat(ptr, points) : qr_scan_opencv(ptr, points);
  if(data.length()){
    return Rcpp::CharacterVector::create(data);
  }
  return R_NilValue;
}

// [[Rcpp::export]]
SEXP cvmat_qrmask(XPtrMat ptr, bool use_wechat = true){
  Mat points;
  std::string data = use_wechat ? qr_scan_wechat(ptr, points) : qr_scan_opencv(ptr, points);
  if (data.length()) {
    cv::Mat img = get_mat(ptr);
    if(!use_wechat)
      points = points.reshape(1, 4);
    for (int i = 0; i < points.size().height; i++) {
      Point pt1 = cv::Point(points.row(i));
      Point pt2 = cv::Point(points.row((i + 1) % 4));
      line(img, pt1, pt2, Scalar(255, 0, 0), 3);
    }
    cv::putText(img, data,
                cv::Point(10, img.rows / 2), //left-middle position
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                CV_RGB(118, 185, 0), //font color
                2);
    ptr.attr("value") = Rcpp::CharacterVector::create(data);
    return ptr;
  }
  return R_NilValue;
}

