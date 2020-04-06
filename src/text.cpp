#include "util.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

using namespace cv;
using namespace cv::dnn;

void decode(const Mat& scores, const Mat& geometry, float scoreThresh,
            std::vector<RotatedRect>& detections,
            std::vector<float>& confidences)
{
#if CV_VERSION_MAJOR < 3 and CV_VERSION_MINOR < 4 and  CV_VERSION_REVISION < 3
  throw std::runtime_error("createBackgroundSubtractorMOG2 requires OpenCV 3 or newer");
#else
  detections.clear();
  CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4);
  CV_Assert(scores.size[0] == 1); CV_Assert(geometry.size[0] == 1);
  CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
  CV_Assert(scores.size[2] == geometry.size[2]);
  CV_Assert(scores.size[3] == geometry.size[3]);

  const int height = scores.size[2];
  const int width = scores.size[3];
  for (int y = 0; y < height; ++y)
  {
    const float* scoresData = scores.ptr<float>(0, 0, y);
    const float* x0_data = geometry.ptr<float>(0, 0, y);
    const float* x1_data = geometry.ptr<float>(0, 1, y);
    const float* x2_data = geometry.ptr<float>(0, 2, y);
    const float* x3_data = geometry.ptr<float>(0, 3, y);
    const float* anglesData = geometry.ptr<float>(0, 4, y);
    for (int x = 0; x < width; ++x)
    {
      float score = scoresData[x];
      if (score < scoreThresh)
        continue;

      // Decode a prediction.
      // Multiple by 4 because feature maps are 4 time less than input image.
      float offsetX = x * 4.0f, offsetY = y * 4.0f;
      float angle = anglesData[x];
      float cosA = std::cos(angle);
      float sinA = std::sin(angle);
      float h = x0_data[x] + x2_data[x];
      float w = x1_data[x] + x3_data[x];

      Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                     offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
      Point2f p1 = Point2f(-sinA * h, -cosA * h) + offset;
      Point2f p3 = Point2f(-cosA * w, sinA * w) + offset;
      RotatedRect r(0.5f * (p1 + p3), Size2f(w, h),
                    -angle * 180.0f / (float)CV_PI);
      detections.push_back(r);
      confidences.push_back(score);
    }
  }
#endif
}


// [[Rcpp::export]]
XPtrMat
  text_detection(XPtrMat input, float confThreshold,float nmsThreshold,
                 int inpWidth, int inpHeight, std::string model, bool draw)
  {
#if CV_VERSION_MAJOR < 3 and CV_VERSION_MINOR < 4 and  CV_VERSION_REVISION < 3
    throw std::runtime_error("createBackgroundSubtractorMOG2 requires OpenCV 3 or newer");
#else
    if (model.empty())
      Rcpp::stop("No model defined");

    Mat inp = get_mat(input);


    // Load network.
    Net net = readNet(model);

    std::vector<Mat> outs;
    std::vector<String> outNames(2);
    outNames[0] = "feature_fusion/Conv_7/Sigmoid";
    outNames[1] = "feature_fusion/concat_3";

    Mat frame, blob;
    std::vector<int> indices;

    frame = inp.clone();

    blobFromImage(frame, blob, 1.0, Size(inpWidth, inpHeight),
                  Scalar(123.68, 116.78, 103.94), true, false);
    net.setInput(blob);
    net.forward(outs, outNames);

    Mat scores = outs[0];
    Mat geometry = outs[1];

    // Decode predicted bounding boxes.
    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
    decode(scores, geometry, confThreshold, boxes, confidences);

    // Apply non-maximum suppression procedure.
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    Rcpp::IntegerVector x1vec(indices.size());
    Rcpp::IntegerVector y1vec(indices.size());
    Rcpp::IntegerVector x2vec(indices.size());
    Rcpp::IntegerVector y2vec(indices.size());
    Rcpp::IntegerVector x3vec(indices.size());
    Rcpp::IntegerVector y3vec(indices.size());
    Rcpp::IntegerVector x4vec(indices.size());
    Rcpp::IntegerVector y4vec(indices.size());


    // Render detections.
    Point2f ratio((float)frame.cols / inpWidth, (float)frame.rows / inpHeight);
    for (size_t i = 0; i < indices.size(); ++i)
    {
      RotatedRect& box = boxes[indices[i]];

      Point2f vertices[4];
      box.points(vertices);
      for (int j = 0; j < 4; ++j)
      {
        vertices[j].x *= ratio.x;
        vertices[j].y *= ratio.y;
      }

      x1vec.at(i) = vertices[0].x;
      y1vec.at(i) = vertices[0].y;
      x2vec.at(i) = vertices[1].x;
      y2vec.at(i) = vertices[1].y;
      x3vec.at(i) = vertices[2].x;
      y3vec.at(i) = vertices[2].y;
      x4vec.at(i) = vertices[3].x;
      y4vec.at(i) = vertices[3].y;

      if (draw)
        for (int j = 0; j < 4; ++j)
          line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 255, 0), 1);
    }

    if (draw) {
      // Put efficiency information.
      std::vector<double> layersTimes;
      double freq = getTickFrequency() / 1000;
      double t = net.getPerfProfile(layersTimes) / freq;
      std::string label = format("Inference time: %.2f ms", t);
      putText(frame, label, Point(0, 15),
              FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
    }


    XPtrMat out = cvmat_xptr(frame);
    out.attr("indices") =  Rcpp::DataFrame::create(
      Rcpp::_["x1"] = x1vec,
      Rcpp::_["y1"] = y1vec,
      Rcpp::_["x2"] = x2vec,
      Rcpp::_["y2"] = y2vec,
      Rcpp::_["x3"] = x3vec,
      Rcpp::_["y3"] = y3vec,
      Rcpp::_["x4"] = x4vec,
      Rcpp::_["y4"] = y4vec
    );

    return out;
#endif
  }
