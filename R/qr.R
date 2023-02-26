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

#' @export
#' @rdname qrcode
qr_scanner <- function(){
  ocv_video(function(im){
    out <- ocv_qrmask(im)
    value <- attr(out, 'value')
    if(length(value)){
      list(out = out, value = value)
    }
  }, stop_on_result = TRUE)
}
