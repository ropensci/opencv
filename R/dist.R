

#' Distance Transform for Binary Files
#'
#' @param image A binary image
#' @param distancetype Type of distance, see cv::DistanceTypes
#' @param masksize Size of the distance transform mask, see cv::DistanceTransformMasks.
#'      In case of the DIST_L1 or DIST_C distance type, the
#'      parameter is forced to 3 because a 3×3 mask gives the same result as 5×5 or any
#'      larger aperture.
#'
#' @return Output image with calculated distances. It is a 8-bit or 32-bit
#'        floating-point, single-channel image of the same size as src .
#' @export
#'
#' @examples
dist_transform <- function(image, distancetype = 3, masksize = 3){

  cv_dist_transform((image), distancetype,masksize, 5)
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
