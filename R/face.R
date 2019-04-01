#' @export
#' @rdname opencv
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
