#include <Rcpp.h>
#include <opencv2/core/core.hpp>


// [[Rcpp::export]]
Rcpp::List enums_types(int maximum = 4){
  std::vector<int> key;
  std::vector<std::string> value;

#ifdef CV_8U
  key.push_back(CV_8U);
  value.push_back("CV_8U");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_8U, i));
    value.push_back("CV_8U" + std::to_string(i));
  }
#endif
#ifdef CV_8S
  key.push_back(CV_8S);
  value.push_back("CV_8S");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_8S, i));
    value.push_back("CV_8S" + std::to_string(i));
  }
#endif
#ifdef CV_16U
  key.push_back(CV_16U);
  value.push_back("CV_16U");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_16U, i));
    value.push_back("CV_16U" + std::to_string(i));
  }
#endif
#ifdef CV_16S
  key.push_back(CV_16S);
  value.push_back("CV_16S");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_16S, i));
    value.push_back("CV_16S" + std::to_string(i));
  }
#endif
#ifdef CV_16F
  key.push_back(CV_16F);
  value.push_back("CV_16F");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_16F, i));
    value.push_back("CV_16F" + std::to_string(i));
  }
#endif
#ifdef CV_32S
  key.push_back(CV_32S);
  value.push_back("CV_32S");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_32S, i));
    value.push_back("CV_32S" + std::to_string(i));
  }
#endif
#ifdef CV_32F
  key.push_back(CV_32F);
  value.push_back("CV_32F");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_32F, i));
    value.push_back("CV_32F" + std::to_string(i));
  }
#endif
#ifdef CV_64F
  key.push_back(CV_64F);
  value.push_back("CV_64F");
  for(int i = 1; i <= maximum; i++){
    key.push_back(CV_MAKETYPE(CV_64F, i));
    value.push_back("CV_64F" + std::to_string(i));
  }
#endif
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("key") = key,
                                      Rcpp::Named("value") = value);
  return out;
}


// [[Rcpp::export]]
Rcpp::List enums_depth(){
  std::vector<int> key;
  std::vector<std::string> value;

#ifdef CV_8U
  key.push_back(CV_8U);
  value.push_back("CV_8U");
#endif
#ifdef CV_8S
  key.push_back(CV_8S);
  value.push_back("CV_8S");
#endif
#ifdef CV_16U
  key.push_back(CV_16U);
  value.push_back("CV_16U");
#endif
#ifdef CV_16S
  key.push_back(CV_16S);
  value.push_back("CV_16S");
#endif
#ifdef CV_16F
  key.push_back(CV_16F);
  value.push_back("CV_16F");
#endif
#ifdef CV_32S
  key.push_back(CV_32S);
  value.push_back("CV_32S");
#endif
#ifdef CV_32F
  key.push_back(CV_32F);
  value.push_back("CV_32F");
#endif
#ifdef CV_64F
  key.push_back(CV_64F);
  value.push_back("CV_64F");
#endif
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("key") = key,
                                      Rcpp::Named("value") = value);
  return out;
}



