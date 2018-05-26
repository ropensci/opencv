#' Face Detection
#'
#' Find faces and eyes in a picture.
#'
#' @export
#' @rdname ocv_face
#' @inheritParams ocv_read
#' @examples unconf <- ocv_read('https://jeroen.github.io/images/unconf18.jpg')
#' faces <- ocv_face(unconf)
#' facemask <- ocv_facemask(unconf)
#' attr(facemask, 'faces')
ocv_face <- function(image){
  facedata <- find_data_xml('haarcascades/haarcascade_frontalface_alt.xml')
  eyedata <- find_data_xml('haarcascades/haarcascade_eye_tree_eyeglasses.xml')
  cvmat_face(image, facedata, eyedata)
}

#' @export
#' @rdname opencv
ocv_facemask <- function(image){
  facedata <- find_data_xml('haarcascades/haarcascade_frontalface_alt.xml')
  cvmat_facemask(image, facedata)
}
