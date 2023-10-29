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
ocv_qr_detect <- function(image, return_image = FALSE, decoder = c("wechat", "quirc")){
  return_image <- as.logical(return_image)
  use_wechat <- match.arg(decoder) == 'wechat'
  cvmat_qr_detect(image, return_image, use_wechat)
}

#' @export
#' @rdname qrcode
#' @param ... parameters passed to [ocv_qr_detect]
qr_scanner <- function(...){
  ocv_video(function(image) ocv_qr_detect(image, ...) , stop_on_result = TRUE)
}
