if(!file.exists("../.deps/opencv/include/opencv4/opencv2/opencv.hpp")){
  unlink("../.deps", recursive = TRUE)
  url <- if(grepl("aarch", R.version$platform)){
    "https://github.com/r-windows/bundles/releases/download/opencv-4.8.1/opencv-4.8.1-clang-aarch64.tar.xz"
  } else if(grepl("clang", Sys.getenv('R_COMPILED_BY'))){
    "https://github.com/r-windows/bundles/releases/download/opencv-4.8.1/opencv-4.8.1-clang-x86_64.tar.xz"
  } else if(getRversion() >= "4.3") {
    "https://github.com/r-windows/bundles/releases/download/opencv-4.8.1/opencv-4.8.1-ucrt-x86_64.tar.xz"
  } else {
    "https://github.com/rwinlib/opencv/archive/refs/tags/v4.8.1.tar.gz"
  }
  download.file(url, basename(url), quiet = TRUE)
  dir.create("../.deps", showWarnings = FALSE)
  untar(basename(url), exdir = "../.deps", tar = 'internal')
  unlink(basename(url))
  setwd("../.deps")
  file.rename(list.files(), 'opencv')
}
