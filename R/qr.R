#' Detect QR codes
#'
#' Detect a QR code from an image. The `ocv_qrtext` function returns the text
#' content from the QR, and `ocv_qrmask` returns an annotated image with the QR
#' area highlighted, and the QR text as an attribute.
#'
#' @export
#' @inheritParams ocv_write
#' @rdname qrcode
#' @param use_wechat use the wechat implementation.
#' @examples
#' png("test.png")
#' plot(qrcode::qr_code("This is a test"))
#' dev.off()
#' ocv_qrtext(ocv_read('test.png'))
#' unlink("test.png")
ocv_qrtext <- function(image, use_wechat = TRUE){
  use_wechat <- as.logical(use_wechat)
  cvmat_qrtext(image, use_wechat)
}

#' @export
#' @rdname qrcode
ocv_qrmask <- function(image, use_wechat = TRUE){
  use_wechat <- as.logical(use_wechat)
  cvmat_qrmask(image, use_wechat)
}

#' @export
#' @rdname qrcode
qr_scanner <- function(){
  ocv_video(ocv_qrmask, stop_on_result = TRUE)
}
