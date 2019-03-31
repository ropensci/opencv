find_data_dir <- function(){
  datapath <- c("/share/opencv", "/share/opencv4", "/share/OpenCV")
  paths <- c(
    system.file('share', package = 'opencv'),
    paste0(data_prefix(), datapath)
  )
  for(x in paths){
    if(file.exists(x))
      return(x)
  }
  stop("Failed to find opencv 'share' directory")
}

find_data_xml <- function(filename){
  normalizePath(file.path(find_data_dir(), filename), mustWork = TRUE)
}
