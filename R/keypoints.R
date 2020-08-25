#' OpenCV keypoints
#'
#' Find key points in images
#' @param image an ocv grayscale image object
#' @param method the type of keypoint detection algorithm
#' @param control a list of arguments passed on to the algorithm
#' @param ... further arguments passed on to ocv_keypoints_options
#' @section FAST algorithm arguments:
#' \itemize{
#' \item threshold threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
#' \item nonmaxSuppression if true, non-maximum suppression is applied to detected corners (keypoints).
#' \item type one of the three neighborhoods as defined in the paper: TYPE_9_16, TYPE_7_12, TYPE_5_8
#' }
#' @section Harris algorithm arguments:
#' \itemize{
#' \item numOctaves the number of octaves in the scale-space pyramid
#' \item corn_thresh the threshold for the Harris cornerness measure
#' \item DOG_thresh the threshold for the Difference-of-Gaussians scale selection
#' \item maxCorners the maximum number of corners to consider
#' \item num_layers the number of intermediate scales per octave
#' }
#' @section BRIEF algorithm arguments:
#' \itemize{
#' \item bytes length of the descriptor in bytes, valid values are: 16, 32 (default) or 64 .
#' \item use_orientation sample patterns using keypoints orientation, disabled by default.
#' }
#' @export
#' @examples
#' mona <- ocv_read('https://jeroen.github.io/images/monalisa.jpg')
#' mona <- ocv_resize(mona, width = 320, height = 477)
#'
#' # FAST-9
#' pts <- ocv_keypoints(mona, method = "FAST", type = "TYPE_9_16", threshold = 40)
#' # Harris
#' pts <- ocv_keypoints(mona, method = "Harris", corners_max = 100)
#' # BRIEF
#' pts <- ocv_keypoints(mona, method = "BRIEF")
#'
#' ocv_destroy(mona)
ocv_keypoints <- function(image, method = c("FAST", "Harris", "BRIEF"), control = ocv_keypoints_options(method, ...), ...){
  method <- match.arg(method)
  params <- control
  if(method == "FAST"){
    cvkeypoints_fast(image, params$threshold, params$nonmax_suppression, params$type)
  }else if(method == "Harris"){
    cvkeypoints_harris(image, params$octaves, params$threshold_corner, params$threshold_dog, params$corners_max, params$layers)
  }else if(method == "BRIEF"){
    cvkeypoints_brief(image, params$bytes, params$use_orientation)
  }
}

ocv_keypoints_options <- function(method = c("FAST", "Harris", "BRIEF"), ...){
  method <- match.arg(method)
  ldots <- list(...)
  if(method == "FAST"){
    params <- list(threshold = 0, nonmaxSuppression = TRUE, type = c("TYPE_9_16", "TYPE_7_12", "TYPE_5_8"))
  }else if(method == "Harris"){
    params <- list(numOctaves = 6, corn_thresh = 0.01, DOG_thresh = 0.01, maxCorners = 5000, num_layers = 4)
  }else if(method == "BRIEF"){
    params <- list(bytes = 32, use_orientation = FALSE)
  }
  for(i in intersect(names(params), names(ldots))){
    params[[i]] <- ldots[[i]]
  }
  if(is.character(params$type)){
    params$type <- recode(params$type, from = c("TYPE_5_8", "TYPE_7_12", "TYPE_9_16"), to = c(0L, 1L, 2L))
  }
  params
}



recode <- function(x, from, to){
  to[match(x, from)]
}
