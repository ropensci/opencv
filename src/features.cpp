#include <Rcpp.h>
#include "util.hpp"
#include <opencv2/core/version.hpp>
#include <opencv2/features2d/features2d.hpp>
#ifdef HAVE_XFEATURES2D
#include <opencv2/xfeatures2d.hpp>
#endif


Rcpp::List keypoints_coords(const std::vector<cv::KeyPoint>& pts){
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
Rcpp::List cvkeypoints_fast(XPtrMat ptr, int threshold = 0, bool nonmaxSuppression = true, int type = 2){
  cv::Mat img;
  cv::cvtColor(get_mat(ptr), img, cv::COLOR_BGR2GRAY);
  std::vector<cv::KeyPoint> keypoints;
#ifndef HAVE_XFEATURES2D
  Rcpp::Rcout << "Keypoint detection disabled as module xfeatures2d from opencv_contrib is not present." << std::endl;
  return keypoints_coords(keypoints);
#else
  // versions < 4 had CV_VERSION_EPOCH instead of CV_VERSION_MAJOR and cv::FastFeatureDetector::DetectorType did not exist (was just an integer)
#if defined(CV_VERSION_EPOCH) || (CV_VERSION_MAJOR < 4)
  int neighbourhood;
#else
  cv::FastFeatureDetector::DetectorType neighbourhood;
#endif
  switch(type){
  case cv::FastFeatureDetector::TYPE_9_16:
    neighbourhood = cv::FastFeatureDetector::TYPE_9_16;
    break;
  case cv::FastFeatureDetector::TYPE_7_12:
    neighbourhood = cv::FastFeatureDetector::TYPE_7_12;
    break;
  case cv::FastFeatureDetector::TYPE_5_8:
    neighbourhood = cv::FastFeatureDetector::TYPE_5_8;
    break;
  default:
    neighbourhood = cv::FastFeatureDetector::TYPE_9_16;
  break;
  }
  cv::xfeatures2d::FASTForPointSet(img, keypoints, threshold, nonmaxSuppression, neighbourhood);
  return keypoints_coords(keypoints);
#endif
}


// [[Rcpp::export]]
Rcpp::List cvkeypoints_harris(XPtrMat ptr,
                              int numOctaves = 6, float corn_thresh = 0.01, float DOG_thresh = 0.01, int maxCorners = 5000, int num_layers = 4){
  cv::Mat img;
  cv::cvtColor(get_mat(ptr), img, cv::COLOR_BGR2GRAY);
  std::vector<cv::KeyPoint> keypoints;
#ifndef HAVE_XFEATURES2D
  Rcpp::Rcout << "Keypoint detection disabled as module xfeatures2d from opencv_contrib is not present." << std::endl;
  return keypoints_coords(keypoints);
#else
  auto featureDetector = cv::xfeatures2d::HarrisLaplaceFeatureDetector::create(numOctaves, corn_thresh, DOG_thresh, maxCorners, num_layers);
  featureDetector->detect(img, keypoints);
  return keypoints_coords(keypoints);
#endif
}


