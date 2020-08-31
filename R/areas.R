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
#' # Polygon area
#' img <- ocv_resize(mona, width = 320, height = 477)
#' pts <- list(x = c(184, 172, 146, 114,  90,  76,  92, 163, 258),
#'             y = c(72,   68,  70,  90, 110, 398, 412, 385, 210))
#' ocv_polygon(img, pts)
#' ocv_polygon(img, pts, crop = TRUE)
#' ocv_polygon(img, pts, convex = TRUE, crop = TRUE)
#'
#' # Bounding box based on points
#' ocv_bbox(img, pts)
#'
#' # Bounding box of non-zero pixel area
#' area <- ocv_polygon(img, pts, color = 0, crop = FALSE)
#' area
#' area <- ocv_bbox(area)
#' area
#'
#' \dontshow{
#' ocv_destroy(mona)
#' ocv_destroy(img)
#' ocv_destroy(area)
#' }
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

#' @export
#' @rdname opencv-area
#' @param pts a list of points with elements x and y
#' @param convex are the points convex
#' @param crop crop the resulting area to its bounding box
#' @param color color for the non-polygon area
ocv_polygon <- function(image, pts, convex = FALSE, crop = FALSE, color = 255){
  stopifnot(is.list(pts) && all(c("x", "y") %in% names(pts)))
  cvmat_polygon(image, pts, convex, crop, color)
}

#' @export
#' @rdname opencv-area
#' @param pts a list of points with elements x and y
ocv_bbox <- function(image, pts){
  if(missing(pts)){
    cvmat_bbox(image)
  }else{
    stopifnot(is.list(pts) && all(c("x", "y") %in% names(pts)))
    cvpoints_bbox(image, pts)
  }
}

#' @export
#' @rdname opencv-area
#' @param pts a list of points with elements x and y
ocv_chull <- function(pts){
  stopifnot(is.list(pts) && all(c("x", "y") %in% names(pts)))
  cvpoints_chull(pts)
}
