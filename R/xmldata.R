find_data_dir <- function(){
  paths <- c(
    system.file('share', package = 'opencv'),
    "/usr/local/share/OpenCV",
    "/usr/share/OpenCV"
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
