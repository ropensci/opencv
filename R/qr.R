#' Detect QR codes
#'
#' Detect a QR code from an image. The `ocv_qr_detect` function returns the text
#' content from the QR, and `ocv_qr_mask` returns an annotated image with the QR
#' area highlighted, and text value as an attribute.
#'
#' @export
#' @inheritParams ocv_write
#' @rdname qrcode
#' @param decoder which decoder implementation to use. The 'wechat' decoder is
#' the default and generally has better performance and fault-tolerance.
#' @examples
#' png("test.png")
#' plot(qrcode::qr_code("This is a test"))
#' dev.off()
#' ocv_qrtext(ocv_read('test.png'))
#' unlink("test.png")
ocv_qr_detect <- function(image, decoder = c("wechat", "quirc")){
  use_wechat <- match.arg(decoder) == 'wechat'
  cvmat_qrtext(image, use_wechat)
}

#' @export
#' @rdname qrcode
ocv_qr_mask <- function(image, decoder = c("wechat", "quirc")){
  use_wechat <- match.arg(decoder) == 'wechat'
  cvmat_qrmask(image, use_wechat)
}

#' @export
#' @rdname qrcode
qr_scanner <- function(){
  ocv_video(ocv_qr_detect, stop_on_result = TRUE)
}
