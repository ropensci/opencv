#include "opencv_types.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/video.hpp>

#include "util.hpp"

using namespace cv;

void finalize_mat( cv::Mat * frame ){
  delete frame;
  frame = NULL;
}

//assume ownership
static XPtrMat cvmat_xptr(cv::Mat *frame){
  XPtrMat ptr(frame);
  ptr.attr("class") = Rcpp::CharacterVector::create("opencv-image");
  return ptr;
}

//copy from stack to heap
static XPtrMat cvmat_xptr(cv::Mat orig){
  cv::Mat * frame = new cv::Mat();
  orig.copyTo(*frame);
  return cvmat_xptr(frame);
}

//opencv has internal refcount system
static cv::Mat get_mat(XPtrMat image){
  if(!Rf_inherits(image, "opencv-image"))
    throw std::runtime_error("Image is not a opencv-image object");
  if(image.get() == NULL)
    throw std::runtime_error("Image is dead");
  return * image.get();
}

// [[Rcpp::export]]
bool cvmat_dead(XPtrMat image){
  return image.get() == NULL;
}

// [[Rcpp::export]]
int cvmat_size(XPtrMat image){
  return image->channels();
}

// [[Rcpp::export]]
XPtrMat cvmat_new(){
  cv::Mat * frame = new cv::Mat();
  return cvmat_xptr(frame);
}

// [[Rcpp::export]]
XPtrMat cvmat_dupe(XPtrMat image){
  return cvmat_xptr(get_mat(image));
}

// [[Rcpp::export]]
XPtrMat cvmat_read(Rcpp::String path){
  const cv::String filename(path);
  cv::Mat frame = imread(filename);
  if(frame.empty())
    throw std::runtime_error("Failed to read file");
  return cvmat_xptr(frame);
}

// [[Rcpp::export]]
XPtrMat cvmat_camera(){
  VideoCapture cap(0);
  if(!cap.isOpened())
    throw std::runtime_error("Failed to start Camera");
  Mat frame;
  cap >> frame;
  cap.release();
  return cvmat_xptr(frame);
}

// [[Rcpp::export]]
std::string cvmat_write(XPtrMat image, std::string path){
  const cv::String filename(path);
  imwrite(filename, get_mat(image));
  return path;
}

// [[Rcpp::export]]
XPtrMat cvmat_resize(XPtrMat ptr, int width = 0, int height = 0){
  cv::Mat input = get_mat(ptr);
  if(!width && !height){
    width = input.cols;
    height = input.rows;
  } else if(width && !height){
    height = round(input.rows * (width / double(input.cols)));
  } else if(!width && height){
    width = round(input.cols * (height / double(input.rows)));
  }
  cv::Mat output;
  cv::resize(input, output, cv::Size(width, height), 0, 0, INTER_LINEAR_EXACT);
  return cvmat_xptr(output);
}

// [[Rcpp::export]]
Rcpp::RawVector cvmat_bitmap(XPtrMat ptr){
  cv::Mat output;
  cv::Mat input = get_mat(ptr);
  cvtColor(input, output, COLOR_BGR2RGB);
  size_t size = output.total();
  size_t channels = output.channels();
  Rcpp::RawVector res(size * channels);
  std::memcpy(res.begin(), output.datastart, size * channels);
  res.attr("dim") = Rcpp::NumericVector::create(channels, output.cols, output.rows);
  return res;
}

// [[Rcpp::export]]
XPtrMat cvmat_face(XPtrMat ptr){
  /* load training data */
  CascadeClassifier face, eyes;
  if(!face.load( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" ))
    throw std::runtime_error("Failed to find haarcascade_frontalface_alt.xml");
  if(!eyes.load( "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ))
    throw std::runtime_error("Failed to find haarcascade_eye_tree_eyeglasses.xml");

  //modify in place
  detectAndDraw(get_mat(ptr), face, eyes, 1, 0);
  return ptr;
}

// [[Rcpp::export]]
XPtrMat cvmat_mog2(XPtrMat ptr) {
  static Ptr<BackgroundSubtractorMOG2> model = createBackgroundSubtractorMOG2();
  model->setVarThreshold(10);
  cv::Mat frame = get_mat(ptr);
  cv::Mat mask, out_frame;
  model->apply(frame, mask);
  //refineSegments(frame, mask, out_frame);
  return cvmat_xptr(mask);
}

// [[Rcpp::export]]
XPtrMat cvmat_knn(XPtrMat ptr) {
  static Ptr<BackgroundSubtractorKNN> model = createBackgroundSubtractorKNN();
  cv::Mat frame = get_mat(ptr);
  cv::Mat mask, out_frame;
  model->apply(frame, mask);
  return cvmat_xptr(mask);
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

// [[Rcpp::export]]
void livestream(Rcpp::Function filter){
  VideoCapture cap(0);
  if(!cap.isOpened())
    throw std::runtime_error("Failed to open Camera");
  Mat image;
  namedWindow("mywindow", 1);
  try {
    for(int i = 0;;i++) {
      cap >> image;
      XPtrMat out(filter(cvmat_xptr(image)));
      imshow("mywindow", get_mat(out));
      waitKey(30);
      Rcpp::checkUserInterrupt();
    }
  } catch(Rcpp::internal::InterruptedException e) { }
  cap.release();
  cv::destroyWindow("mywindow");
}
