#' OpenCV
#'
#' Utilities to mess around with OpenCV.
#'
#' @export
#' @rdname opencv
#' @importFrom Rcpp sourceCpp
#' @useDynLib opencv
#' @param path image file such as png or jpeg
#' @examples png('bg.png', width = 1280, height = 720)
#' par(ask=FALSE)
#' qplot(speed, dist, data = cars, geom = c("smooth", "point"))
#' dev.off()
#' bg <- ocv_read('bg.png')
#' unlink('pg.png')
#' ocv_video(function(input){
#'   mask <- ocv_mog2(input)
#'   return(ocv_copyto(input, bg, mask))
#' })
#'
#' # Overlay face filter
#' ocv_video(function(input){
#'   mask <- ocv_facemask(input)
#'   ocv_copyto(input, bg, mask)
#' })
ocv_read <- function(path){
  path <- normalizePath(path, mustWork = TRUE)
  cvmat_read(path)
}

#' @export
#' @rdname opencv
#' @param image a ocv image object
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
`print.opencv-image` <- function(x, ...){
  viewer <- getOption("viewer")
  is_knit_image <- isTRUE(getOption('knitr.in.progress'))
  if(!is_knit_image && is.function(viewer) && !cvmat_dead(x)){
    tmp <- file.path(tempdir(), paste0("preview.png"))
    ocv_write(x, path = tmp)
    viewer(tmp)
  }
  base::print.default(x)
  #TODO: copy from magick
}

#' @export
#' @rdname opencv
#' @param width output width in pixels
#' @param height output height in pixels
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
ocv_facemask <- function(image){
  cvmat_facemask(image)
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
#' @param target the output image
#' @param mask only copy pixels from the mask
ocv_copyto <- function(image, target, mask){
  cvmat_copyto(image, target, mask)
}

#' @export
#' @rdname opencv
#' @param filter an R function that takes and returns an opecv image
ocv_video <- function(filter){
  if(!is.function(filter))
    stop("Filter must be a function")
  livestream(function(image){
    if(!inherits(image, 'opencv-image'))
      stop("Image must be opencv-image")
    out <- filter(image)
    if(!inherits(out, 'opencv-image'))
      stop("Output must be opencv-image")
    return(out)
  })
}

#' @importFrom magrittr %>%
#' @export
magrittr::`%>%`

