enums <- new.env()


.onAttach <- function(libname, pkgname){
  enums$type <- enums_types()
  enums$depth <- enums_depth()
}

enum_label <- function(type, id){
  kv <- enums[[type]]
  recoder(id, from = kv$key, to = kv$value)
}
enum_id <- function(type, label){
  kv <- enums[[type]]
  recoder(label, from = kv$value, to = kv$key)
}

.onLoad <- function(libname, pkgname){
  if(is_check()){
    set_num_threads(2)
  }
}

is_macos <- function(){
  grepl("darwin", R.Version()$platform)
}

is_iterm2 <- function(){
  identical(Sys.getenv('TERM_PROGRAM'), 'iTerm.app')
}

is_check <- function(){
  grepl('opencv.Rcheck', getwd(), fixed = TRUE)
}

is_mojave <- function(){
  ver <- utils::tail(strsplit(utils::sessionInfo()$running, ' ')[[1]], 1)
  as.numeric_version(ver) >= "10.14"
}

is_rstudio <- function(){
  Sys.getenv('RSTUDIO') == "1"
}
