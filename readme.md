# Vision and Graphics

Some experiments combining vision and graphics.

## Installation

First install opencv:

```sh
brew install opencv
```

And then the usual:

```r
devtools::install_github("ropenscilabs/opencv")
library(opencv)
```

## Background Filter

```r
png('bg.png', width = 1280, height = 720)
par(ask=FALSE)
qplot(speed, dist, data = cars, geom = c("smooth", "point"))
dev.off()
bg <- ocv_read('bg.png')
unlink('pg.png')
ocv_video(function(input){
  mask <- ocv_mog2(input)
  return(ocv_copyto(input, bg, mask))
})
```

## Face Detection

```r
# Overlay face filter
ocv_video(function(input){
  mask <- ocv_facemask(input)
  ocv_copyto(input, bg, mask)
})
```
