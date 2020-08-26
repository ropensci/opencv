#include <Rcpp.h>
#include <opencv2/core/version.hpp>

// [[Rcpp::export]]
std::string cvversion(){
  return CV_VERSION;
}
