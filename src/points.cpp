#include "util.hpp"

using namespace cv;
using namespace std;

// [[Rcpp::export]]
XPtrMat cvmat_markers(XPtrMat ptr){
  Mat gray;
  Mat image = get_mat(ptr);
  cvtColor(image, gray, COLOR_BGR2GRAY);
  vector<Point2f> points;
  goodFeaturesToTrack(gray, points, 500, 0.01, 10);

  TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
  cornerSubPix(gray, points, Size(10,10), Size(-1,-1), termcrit);

  for(size_t i = 0; i < points.size(); i++ ) {
    circle( image, points[i], 3, Scalar(0,255,0), -1, 8);
  }
  return ptr;
}

