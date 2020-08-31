recoder <- function(x, from = c(), to = c()){
  if(length(x) == 0){
    return(x)
  }
  stopifnot(length(from) == length(to))
  nongiven <- unique(x[!is.na(x)])
  nongiven <- setdiff(nongiven, from)
  if(length(nongiven) > 0) {
    from <- append(x = from, values = nongiven)
    to   <- append(x = to, values = nongiven)
  }
  to[match(x, from)]
}



