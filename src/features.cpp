#include <Rcpp.h>
#include "util.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

Rcpp::List keypoints_coords(const std::vector<KeyPoint>& pts){
  std::vector<int> x;
  std::vector<int> y;
  for (size_t i = 0; i < pts.size(); ++i){
    x.push_back(pts[i].pt.x);
    y.push_back(pts[i].pt.y);
  }
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("x") = x,
                                      Rcpp::Named("y") = y);
  return out;
}


// [[Rcpp::export]]
Rcpp::List cvkeypoints_fast(XPtrMat ptr, int threshold = 0, bool nonmaxSuppression = true, int type = cv::FastFeatureDetector::TYPE_9_16){
  cv::Mat img;
  cv::cvtColor(get_mat(ptr), img, cv::COLOR_BGR2GRAY);
  std::vector<cv::KeyPoint> keypoints;
  cv::FastFeatureDetector::DetectorType neighbourhood;
  switch(type){
    case FastFeatureDetector::TYPE_9_16:
      neighbourhood = FastFeatureDetector::TYPE_9_16;
      break;
    case FastFeatureDetector::TYPE_7_12:
      neighbourhood = FastFeatureDetector::TYPE_7_12;
      break;
    case FastFeatureDetector::TYPE_5_8:
      neighbourhood = FastFeatureDetector::TYPE_5_8;
      break;
    default:
      neighbourhood = FastFeatureDetector::TYPE_9_16;
      break;
  }
  cv::xfeatures2d::FASTForPointSet(img, keypoints, threshold, nonmaxSuppression, neighbourhood);
  return keypoints_coords(keypoints);
}


// [[Rcpp::export]]
Rcpp::List cvkeypoints_brief(XPtrMat ptr, int bytes = 32, bool use_orientation = false){
  cv::Mat img;
  cv::cvtColor(get_mat(ptr), img, cv::COLOR_BGR2GRAY);
  std::vector<cv::KeyPoint> keypoints;
  auto featureDetector = cv::xfeatures2d::BriefDescriptorExtractor::create(bytes, use_orientation);
  featureDetector->detect(img, keypoints);
  return keypoints_coords(keypoints);
}



