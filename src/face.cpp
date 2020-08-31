#include "util.hpp"

using namespace cv;

// [[Rcpp::export]]
XPtrMat cvmat_face(XPtrMat ptr, const char * facedata, const char * eyedata){
  /* load training data */
  CascadeClassifier face, eyes;
  if(!face.load( facedata ))
    throw std::runtime_error(std::string("Failed to load: ") + facedata);
  if(!eyes.load( eyedata ))
    throw std::runtime_error(std::string("Failed to load: ") + eyedata);

  //modify in place
  detectAndDraw(get_mat(ptr), face, eyes, 1, 0);
  return ptr;
}

// [[Rcpp::export]]
XPtrMat cvmat_facemask(XPtrMat ptr, const char * facedata){
  /* load training data */
  CascadeClassifier face;
  if(!face.load( facedata ))
    throw std::runtime_error(std::string("Failed to load: ") + facedata);

  Mat gray;
  Mat input = get_mat(ptr);
  cvtColor( input, gray, COLOR_BGR2GRAY );
  equalizeHist(gray, gray);
  std::vector<Rect> faces;
  face.detectMultiScale( gray, faces, 1.1, 2, 0
                           //|CASCADE_FIND_BIGGEST_OBJECT
                           //|CASCADE_DO_ROUGH_SEARCH
                           |CASCADE_SCALE_IMAGE,
                           Size(30, 30) );

  Mat mask(gray.size(), gray.type(), Scalar::all(0));
  Rcpp::IntegerVector rvec(faces.size());
  Rcpp::IntegerVector xvec(faces.size());
  Rcpp::IntegerVector yvec(faces.size());
  for ( size_t i = 0; i < faces.size(); i++ ) {
    Point center;
    Rect r = faces.at(i);
    center.x = cvRound((r.x + r.width*0.5));
    center.y = cvRound((r.y + r.height*0.5));
    int radius = cvRound((r.width + r.height)*0.25);
    circle( mask, center, radius, Scalar::all(255), -1);
    rvec.at(i) = radius;
    xvec.at(i) = center.x;
    yvec.at(i) = center.y;
  }
  XPtrMat out = cvmat_xptr(mask);
  out.attr("faces") = Rcpp::DataFrame::create(
    Rcpp::_["radius"] = rvec,
    Rcpp::_["x"] = xvec,
    Rcpp::_["y"] = yvec
  );
  return out;
}

// [[Rcpp::export]]
XPtrMat cvmat_mog2(XPtrMat ptr) {
#if (defined(CV_VERSION_EPOCH) && (CV_VERSION_EPOCH < 3)) || (CV_VERSION_MAJOR < 3)
  throw std::runtime_error("createBackgroundSubtractorMOG2 requires OpenCV 3 or newer");
#else
  static Ptr<BackgroundSubtractorMOG2> model = createBackgroundSubtractorMOG2();
  model->setVarThreshold(10);
  cv::Mat frame = get_mat(ptr);
  cv::Mat mask, out_frame;
  model->apply(frame, mask);
  //refineSegments(frame, mask, out_frame);
  return cvmat_xptr(mask);
#endif
}

// [[Rcpp::export]]
XPtrMat cvmat_knn(XPtrMat ptr) {
#if (defined(CV_VERSION_EPOCH) && (CV_VERSION_EPOCH < 3)) || (CV_VERSION_MAJOR < 3)
  throw std::runtime_error("createBackgroundSubtractorKNN requires OpenCV 3 or newer");
#else
  static Ptr<BackgroundSubtractorKNN> model = createBackgroundSubtractorKNN();
  cv::Mat frame = get_mat(ptr);
  cv::Mat mask, out_frame;
  model->apply(frame, mask);
  return cvmat_xptr(mask);
#endif
}

// [[Rcpp::export]]
XPtrMat cvmat_edges(XPtrMat ptr) {
  cv::Mat edges;
  cv::Mat frame = get_mat(ptr);
  cvtColor(frame, edges, COLOR_BGR2GRAY);
  GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
  Canny(edges, edges, 0, 30, 3);
  return cvmat_xptr(edges);
}
