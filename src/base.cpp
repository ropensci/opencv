#include "util.hpp"

#define xstr(s) str(s)
#define str(s) #s

using namespace cv;

void finalize_mat( cv::Mat * frame ){
  delete frame;
  frame = NULL;
}

//assume ownership
XPtrMat cvmat_xptr(cv::Mat *frame){
  XPtrMat ptr(frame);
  ptr.attr("class") = Rcpp::CharacterVector::create("opencv-image");
  return ptr;
}

//copy from stack to heap
XPtrMat cvmat_xptr(cv::Mat orig){
  cv::Mat * frame = new cv::Mat();
  orig.copyTo(*frame);
  return cvmat_xptr(frame);
}

//opencv has internal refcount system
cv::Mat get_mat(XPtrMat image){
  if(!image.inherits("opencv-image"))
    throw std::runtime_error("Image is not a opencv-image object");
  if(image.get() == NULL)
    throw std::runtime_error("Image has been destroyed");
  return * image.get();
}

// [[Rcpp::export]]
void cvmat_destroy(XPtrMat image){
  if(!image.inherits("opencv-image"))
    throw std::runtime_error("Image is not a opencv-image object");
  image.release();
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
  cv::resize(input, output, cv::Size(width, height));
  return cvmat_xptr(output);
}

// [[Rcpp::export]]
XPtrMat cvmat_raw_bgr(Rcpp::RawVector image, int width = 0, int height = 0){
  // 8bit Blue Green Red
  if(image.length() != width * height * 3){
    throw std::runtime_error("cvmat_raw_bgr requires data with 3 channels");
  }
  std::vector<uchar> x = Rcpp::as<std::vector<uchar>>(image);
  cv::Mat output(height, width, CV_8UC3, x.data());
  return cvmat_xptr(output);
}


// [[Rcpp::export]]
XPtrMat cvmat_raw_bw(Rcpp::RawVector image, int width = 0, int height = 0){
  // 8bit 1 channel (e.g. Black/White or Greyscale)
  if(image.length() != width * height * 1){
    throw std::runtime_error("cvmat_raw_bw requires data with 1 channel (e.g. grey or just black/white)");
  }
  std::vector<uchar> x = Rcpp::as<std::vector<uchar>>(image);
  cv::Mat output(height, width, CV_8U, x.data());
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
XPtrMat cvmat_copyto(XPtrMat from, XPtrMat to, XPtrMat mask) {
  XPtrMat out = cvmat_xptr(get_mat(to));
  get_mat(from).copyTo(get_mat(out), get_mat(mask));
  return out;
}

// [[Rcpp::export]]
Rcpp::List cvmat_info(XPtrMat image){
  return Rcpp::List::create(
    Rcpp::_["width"] = get_mat(image).cols,
    Rcpp::_["height"] = get_mat(image).rows,
    Rcpp::_["channels"] = get_mat(image).channels(),
    Rcpp::_["depth"] = get_mat(image).depth(),
    Rcpp::_["type"] = get_mat(image).type()
  );
}

// [[Rcpp::export]]
void cvmat_display(XPtrMat ptr){
  namedWindow("mywindow", 1);
  imshow("mywindow", get_mat(ptr));
  try {
    for(int i = 0;;i++) {
      if(waitKey(30) >= 0 || cv::getWindowProperty("mywindow", 0) < 0)
        break;
      Rcpp::checkUserInterrupt();
    }
  } catch(Rcpp::internal::InterruptedException e) { }
  cv::destroyWindow("mywindow");
  cv::waitKey(1);
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
      if(waitKey(30) >= 0 || cv::getWindowProperty("mywindow", 0) < 0)
        break;
      Rcpp::checkUserInterrupt();
    }
  } catch(Rcpp::internal::InterruptedException e) { }
  cap.release();
  cv::destroyWindow("mywindow");
  cv::waitKey(1);
}

// [[Rcpp::export]]
Rcpp::String data_prefix(){
#ifdef _DATA_PREFIX
  return Rcpp::String(xstr(_DATA_PREFIX));
#endif
  return Rcpp::String("/usr");
}

// [[Rcpp::export]]
int set_num_threads(int n = 0){
  if(n > 0)
    setNumThreads(n);
  return getNumThreads();
}
