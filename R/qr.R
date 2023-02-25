#' Detect QR codes
#'
#' Detect a QR code
#'
#' @export
#' @inheritParams ocv_write
#' @rdname qrcode
ocv_qrtext <- function(image){
  cvmat_qrtext(image)
}

#' @export
#' @rdname qrcode
#' @examples \dontrun{
#' # QR code scanner
#' ocv_camera(ocv_qrmask)
#' }
ocv_qrmask <- function(image){
  cvmat_qrmask(image)
}
