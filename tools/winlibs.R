# We need https
if(getRversion() < "3.3.0") setInternet2()
VERSION <- commandArgs(TRUE)

# Downloads libssh2 + dependencies
if(!file.exists(sprintf("../windows/opencv-%s/include/opencv4/opencv2/opencv.hpp", VERSION))){
  download.file(sprintf("https://github.com/rwinlib/opencv/archive/v%s.zip", VERSION), "lib.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("lib.zip", exdir = "../windows")
  unlink("lib.zip")
}
