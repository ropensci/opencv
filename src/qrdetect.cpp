#include "util.hpp"
using namespace cv;

#ifdef HAVE_WECHATQR
#include <opencv2/wechat_qrcode.hpp>
static std::string qr_scan_wechat(XPtrMat ptr, Mat &points){
  static wechat_qrcode::WeChatQRCode qrDet = wechat_qrcode::WeChatQRCode();
  std::vector<Mat> pointsvec;
  std::vector<std::string> data = qrDet.detectAndDecode(get_mat(ptr), pointsvec);
  if(data.size() > 0){
    points = pointsvec.at(0);
    return data.at(0);
  }
  return std::string();
}
#endif

static std::string qr_scan_opencv(XPtrMat ptr, Mat &points){
#ifdef HAVE_QUIRC
  static QRCodeDetector qrDet = QRCodeDetector();
  return qrDet.detectAndDecode(get_mat(ptr), points);
#else
  throw std::runtime_error("QR scanner requires OpenCV 3.4.4 or newer");
#endif
}

// [[Rcpp::export]]
SEXP cvmat_qr_detect(XPtrMat ptr, bool return_image = false, bool use_wechat = true){
  Mat points;
#ifdef HAVE_WECHATQR
  std::string data = use_wechat ? qr_scan_wechat(ptr, points) : qr_scan_opencv(ptr, points);
#else
  std::string data = qr_scan_opencv(ptr, points);
#endif
  if(!data.length())
    return R_NilValue;
  if(!use_wechat)
    points = points.reshape(1, 4);
  int len = points.size().height;
  Rcpp::IntegerMatrix corners(len, 2);
  for (int i = 0; i < len; i++) {
    Point pt = cv::Point(points.row(i));
    corners[i] = pt.x;
    corners[i+len] = pt.y;
  }
  if(!return_image){
    Rcpp::CharacterVector out(data);
    out.attr("points") = corners;
    return out;
  }
  cv::Mat img = get_mat(ptr);
  for (int i = 0; i < len; i++) {
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
  ptr.attr("points") = corners;
  return ptr;
}

