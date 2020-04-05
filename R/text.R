#' ocv_text text detection in images using EAST
#'
#' @description OpenCV sample using EAST (Efficient and Accurate Scene Text
#'   Detector) to detect text in images. Requires the EAST pb-model not included
#'   in the package.
#'
#' @param image Path to input image or video file. Skip this argument to capture
#'   frames from a camera.
#' @param width Preprocess input image by resizing to a specific width. It
#'   should be multiple by 32.
#' @param height Preprocess input image by resizing to a specific height. It
#'   should be multiple by 32.
#' @param thrs Confidence threshold.
#' @param nms Non-maximum suppression threshold.
#' @param model Path to a binary .pb file contains trained network.
#' @param draw Draws visual output to the image.
#'
#' @examples
#' \dontrun{
#' url <- paste0('https://upload.wikimedia.org/wikipedia/commons/6/6f/',
#'               'Keep-calm-and-carry-on-scan.jpg')
#' fl <- ocv_read(url)
#'
#' ocv_text(image = fl, thrs =  0.7,  nms = 0.3,
#'          model = "frozen_east_text_detection.pb")
#' }
#'
#' @references
#' https://github.com/opencv/opencv/blob/master/samples/dnn/text_detection.cpp
#'
#' @export
ocv_text <- function(image, thrs = 0.5, nms = 20, width = 320, height = 320,
                     model, draw = FALSE){

  if(missing(model))
    stop("requires a model pb-file")

  model <- path.expand(model)

  text_detection(input = image,
                 confThreshold = thrs,
                 nmsThreshold = nms,
                 inpWidth = width,
                 inpHeight = height,
                 model = model,
                 draw = draw)
}

