

#' Distance Transform for Binary Files
#'
#' @param matrix
#'
#' @return
#' @export
#'
#' @examples
dist_transform <- function(image){

  cv_dist_transform((image), 1,3, 3)
}

#' Grayscale version of image
#'
#' @param image
#'
#' @return
#' @export
#'
ocv_grayscale <- function(image){
  cv_grayscale(image)
}



#' Binarize grayscale version of image
#'
#' @param image
#'
#' @return
#' @export
#'
ocv_binarize <- function(image){
  cv_binary(image)
}
