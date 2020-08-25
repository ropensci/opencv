#' OpenCV area manipulation
#'
#' Manipulate image regions
#' @name opencv-area
#' @rdname opencv-area
#' @param image an ocv image object
#' @param x horizontal location
#' @param y vertical location
#' @param width width of the area
#' @param height height of the area
#' @export
#' @examples
#' mona <- ocv_read('https://jeroen.github.io/images/monalisa.jpg')
#'
#' # Rectangular area
#' ocv_rectangle(mona, x = 400, y = 300, height = 300, width = 350)
#' ocv_rectangle(mona, x = 0, y = 100, height = 200)
#' ocv_rectangle(mona, x = 500, y = 0, width = 75)
#'
#' ocv_destroy(mona)
ocv_rectangle <- function(image, x = 0L, y = 0L, width, height){
  size <- ocv_info(image)
  if(missing(width)){
    width <- size$width
  }
  if(missing(height)){
    height <- size$height
  }
  cvmat_rect(image, x, y, width, height)
}
