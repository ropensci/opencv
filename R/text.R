#' @export
#' @rdname opencv
#' @param image Path to input image or video file. Skip this argument to capture frames from a camera.
#' @param width Preprocess input image by resizing to a specific width. It should be multiple by 32.
#' @param height Preprocess input image by resizing to a specific height. It should be multiple by 32.
#' @param thrs Confidence threshold.
#' @param nms Non-maximum suppression threshold.
#'
#' @param model Path to a binary .pb file contains trained network.
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

