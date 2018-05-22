#' OpenCV
#'
#' Utilities to read and write images with OpenCV.s
#'
#' @export
#' @rdname opencv
#' @importFrom Rcpp sourceCpp
#' @useDynLib opencv
ocv_read <- function(path){
  path <- normalizePath(path, mustWork = TRUE)
  cvmat_read(path)
}

#' @export
#' @rdname opencv
ocv_write <- function(image, path){
  path <- normalizePath(path, mustWork = FALSE)
  cvmat_write(image, path)
}

#' @export
#' @rdname opencv
ocv_bitmap <- function(image){
  cvmat_bitmap(image)
}

#' @export
#' @rdname opencv
ocv_edges <- function(image){
  cvmat_edges(image)
}

#' @export
#' @rdname opencv
ocv_camera <- function(){
  cvmat_camera()
}

#' @export
#' @rdname opencv
#' @param x passed to [print]
`print.opencv-image` <- function(x, ...){
  viewer <- getOption("viewer")
  is_knit_image <- isTRUE(getOption('knitr.in.progress'))
  if(!is_knit_image && is.function(viewer) && !cvmat_dead(x)){
    tmp <- file.path(tempdir(), paste0("preview.png"))
    ocv_write(x, path = tmp)
    viewer(tmp)
  }
  #TODO: copy from magick
}

#' @export
#' @rdname opencv
ocv_resize <- function(image, width = 0, height = 0){
  cvmat_resize(image, as.integer(width), as.integer(height))
}

#' @export
#' @rdname opencv
ocv_face <- function(image){
  cvmat_face(image)
}

#' @export
#' @rdname opencv
ocv_mog2 <- function(image){
  cvmat_mog2(image)
}

#' @export
#' @rdname opencv
ocv_knn <- function(image){
  cvmat_knn(image)
}

#' @export
#' @rdname opencv
ocv_video <- function(filter){
  if(!is.function(filter))
    stop("Filter must be a function")
  livestream(filter)
}

#' @importFrom magrittr %>%
#' @export
magrittr::`%>%`

