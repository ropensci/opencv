# Vision and Graphics

Some experiments combining vision and graphics.

## Installation

First install opencv:

```sh
brew install opencv
```

And then install the R bindings:

```r
devtools::install_github("ropenscilabs/opencv")
library(opencv)
```

Works best in the terminal (rstudio doesn't like the popup window).

## Basic stuff:

Basic face detection:

```r
ocv_video(ocv_face)
```

Edge detection:


```r
ocv_video(ocv_edges)
```

## Combine with Graphics

Replaces the background with a plot:

```r
png('bg.png', width = 1280, height = 720)
par(ask=FALSE)
print(ggplot2::qplot(speed, dist, data = cars, geom = c("smooth", "point")))
dev.off()
bg <- ocv_read('bg.png')
unlink('pg.png')
ocv_video(function(input){
  mask <- ocv_mog2(input)
  return(ocv_copyto(input, bg, mask))
})
```

Put your face in the plot:

```r
# Overlay face filter
ocv_video(function(input){
  mask <- ocv_facemask(input)
  ocv_copyto(input, bg, mask)
})
```
