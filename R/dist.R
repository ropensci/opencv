

#' Distance Transform for Binary Files
#'
#' @param image An external pointer to an OpenCV binary image
#' @param distancetype Type of distance, see cv::DistanceTypes
#' @param masksize Size of the distance transform mask, see cv::DistanceTransformMasks.
#'      In case of the DIST_L1 or DIST_C distance type, the
#'      parameter is forced to 3 because a 3×3 mask gives the same result as 5×5 or any
#'      larger aperture.
#' @param dsttype Type of output image. It can be CV_8U (0) or CV_32F (5).
#'      Type CV_8U can be used
#'      only for the first variant of the function and distanceType == DIST_L1.
#'
#' @return Output image with calculated distances. It is a 8-bit or 32-bit
#'        floating-point, single-channel image of the same size as src .
#' @export
#'
#' @examples
dist_transform <- function(image, distancetype = 3, masksize = 3, dsttype = 5){

  cv_dist_transform((image), distancetype,masksize, dsttype)
}

#' Grayscale version of image
#'
#' This is not a robust function, it is given as an example.
#'
#' @param image A three-channel image
#'
#' @return An external pointer to a grayscale image.
#' @export
#'
ocv_grayscale <- function(image){
  cv_grayscale(image)
}



#' Binarize grayscale version of image
#'
#' This is not a robust function, it is only given as an example.
#'
#' @param image A grayscale image
#'
#' @return An external point to a binary image
#' @export
#'
ocv_binarize <- function(image){
  cv_binary(image)
}


#' Distance transformation for a 3 channel image
#'
#' This is not a robust function, it is only given as an example.
#' This takes an OpenCV image, transforms it into a binary image, then
#' computes and returns the distance transform of that image.
#'
#' @param image An external pointer to an OpenCV image.
#'
#' @return An external pointer to an image of the distance transform.
#' @export
#'
ocv_output_dist <- function(image){
  dist_transform(cv_binary(cv_grayscale(image)))
}
