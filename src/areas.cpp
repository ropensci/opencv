#include "util.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

std::vector<cv::Point> as_points(Rcpp::List pts){
  std::vector<cv::Point> points;
  std::vector<int> x = Rcpp::as<std::vector<int>>(pts["x"]);
  std::vector<int> y = Rcpp::as<std::vector<int>>(pts["y"]);
  for (size_t i = 0; i < x.size(); ++i){
    cv::Point2i pt(x[i], y[i]);
    points.push_back(pt);
  }
  return points;
}

Rcpp::List points_coords(const std::vector<cv::Point>& pts){
  std::vector<int> x;
  std::vector<int> y;
  for (size_t i = 0; i < pts.size(); ++i){
    x.push_back(pts[i].x);
    y.push_back(pts[i].y);
  }
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("x") = x,
                                      Rcpp::Named("y") = y);
  return out;
}

// [[Rcpp::export]]
XPtrMat cvmat_rect(XPtrMat ptr, int x = 0, int y = 0, int width = 0, int height = 0){
  cv::Mat img = get_mat(ptr);
  cv::Rect roi = cv::Rect(x, y, width, height);
  cv::Mat output = img(roi);
  return cvmat_xptr(output);
}

// [[Rcpp::export]]
XPtrMat cvmat_bbox(XPtrMat ptr){
#if defined(CV_VERSION_EPOCH) && (CV_VERSION_EPOCH < 3)
  Rcpp::Rcout << "ocv_bbox works only if you provide points in your version of opencv, returning original image" << std::endl;
  return ptr;
#else
  cv::Mat img = get_mat(ptr);
  cv::Mat output;
  cv::Rect roi;
  if(img.channels() > 1){
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    roi = cv::boundingRect(img_gray);
  }else{
    roi = cv::boundingRect(img);
  }
  output = img(roi);
  return cvmat_xptr(output);
#endif
}

// [[Rcpp::export]]
XPtrMat cvpoints_bbox(XPtrMat ptr, Rcpp::List pts){
  cv::Mat img = get_mat(ptr);
  auto points = as_points(pts);
  cv::Rect roi = cv::boundingRect(points);
  cv::Mat output = img(roi);
  return cvmat_xptr(output);
}

// [[Rcpp::export]]
Rcpp::List cvpoints_chull(Rcpp::List pts){
  std::vector<cv::Point> points = as_points(pts);
  if(points.size() == 0){
    return points_coords(points);
  }
  std::vector<cv::Point> points_chull;
  cv::convexHull(points, points_chull);
  return points_coords(points_chull);
}


// [[Rcpp::export]]
XPtrMat cvmat_polygon(XPtrMat ptr, Rcpp::List pts, bool convex = false, bool crop = false, int color = 255){
  auto points = as_points(pts);
  cv::Mat img = get_mat(ptr);
  cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8U);
  if(convex){
    cv::fillConvexPoly(mask, points, cv::Scalar(255, 255, 255));
  }else{
    std::vector<std::vector<cv::Point>> polygons;
    polygons.push_back(points);
    cv::fillPoly(mask, polygons, cv::Scalar(255, 255, 255));
  }
  cv::Mat area(img.rows, img.cols, img.type());
  area.setTo(cv::Scalar(color, color, color));
  img.copyTo(area, mask);
  if(crop){
    cv::Rect roi = cv::boundingRect(points);
    return cvmat_xptr(area(roi));
  }
  return cvmat_xptr(area);
}

