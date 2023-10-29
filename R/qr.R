#' Detect and Decode a QR code
#'
#' Detect and decode a QR code from an image or camera. By default it returns
#' the text value from the QR code if detected, or NULL if no QR was found. If
#' `draw = TRUE` then it returns an annotated image with the position and
#' value of the QR drawn into the image, and qr text value as an attribute.
#'
#' The `qr_scanner` function tries to open the camera device (if available on
#' your computer) and repeats calling [ocv_qr_detect] until it has found a QR
#' code, or until interrupted.
#'
#' OpenCV has two separate QR decoders. The 'wechat' decoder was added in
#' libopencv 4.5.2 and generally has better performance and fault-tolerance.
#' The old 'quirc' decoder is available on some older versions of libopencv as
#' a plug-in, but many Linux distros did not include it. If you get an error
#' *Library QUIRC is not linked. No decoding is performed.* this sadly means
#' your Linux distribution is too old and does not support QR decoding.
#'
#' @export
#' @rdname qrcode
#' @inheritParams ocv_write
#' @param draw if TRUE, the function returns an annotated image showing
#' the position and value of the QR code.
#' @param decoder which decoder implementation to use, see details.
#' @returns if a QR code is detected, this returns either the text value of the QR,
#' or if `draw` it returns the annotated image, with the value as an attribute.
#' Returns NULL if no QR was found in the image.
#' @examples
#' png("test.png")
#' plot(qrcode::qr_code("This is a test"))
#' dev.off()
#' ocv_qr_detect(ocv_read('test.png'))
#' unlink("test.png")
ocv_qr_detect <- function(image, draw = FALSE, decoder = c("wechat", "quirc")){
  draw <- as.logical(draw)
  use_wechat <- match.arg(decoder) == 'wechat'
  cvmat_qr_detect(image, draw, use_wechat)
}

#' @export
#' @rdname qrcode
qr_scanner <- function(draw = FALSE, decoder = c("wechat", "quirc")){
  ocv_video(function(image) {
    ocv_qr_detect(image = image, draw = draw, decoder = decoder)
  }, stop_on_result = TRUE)
}
