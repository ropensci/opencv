#include "opencv_types.h"
void detectAndDraw( cv::Mat img, cv::CascadeClassifier& cascade,
                    cv::CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );
cv::Mat get_mat(XPtrMat image);
XPtrMat cvmat_xptr(cv::Mat *frame);
XPtrMat cvmat_xptr(cv::Mat orig);
