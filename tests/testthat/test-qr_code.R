test_that("qr scanner works", {
  skip_if(identical(Sys.info()[["sysname"]], 'Linux') && ocv_version() < "4.5.2")
  img <- ocv_read('qrtest.jpg')
  expect_s3_class(img, "opencv-image")
  txt <- ocv_qr_detect(img)
  outimg <- ocv_qr_detect(img, draw = TRUE)
  expect_s3_class(outimg, "opencv-image")
  expect_equal(c(txt), "https://www.r-project.org")
})
