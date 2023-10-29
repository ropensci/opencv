test_that("qr scanner works", {
  skip_if(ocv_version() < "4.5.2")
  img <- ocv_read('qrtest.jpg')
  expect_s3_class(img, "opencv-image")
  txt <- ocv_qr_detect(img)
  expect_equal(txt, "https://www.r-project.org")
})
