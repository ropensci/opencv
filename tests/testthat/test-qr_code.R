test_that("qr scanner works", {
  img <- ocv_read('qrtest.jpg')
  expect_s3_class(img, "opencv-image")
  txt <- ocv_qrtext(img)
  expect_equal(txt, "https://www.r-project.org")
})
