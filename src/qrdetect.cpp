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
  static QRCodeDetector qrDet = QRCodeDetector();
  return qrDet.detectAndDecode(get_mat(ptr), points);
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
  if(!return_image)
    return Rcpp::CharacterVector::create(data);
  cv::Mat img = get_mat(ptr);
  if(!use_wechat)
    points = points.reshape(1, 4);
  int len = points.size().height;
  Rcpp::IntegerMatrix corners(len, 2);
  for (int i = 0; i < len; i++) {
    Point pt1 = cv::Point(points.row(i));
    Point pt2 = cv::Point(points.row((i + 1) % 4));
    line(img, pt1, pt2, Scalar(255, 0, 0), 3);
    corners[i] = pt1.x;
    corners[i+len] = pt1.y;
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

