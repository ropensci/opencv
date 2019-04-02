.onAttach <- function(libname, pkgname){
  if(is_macos() && is_iterm2()){
    version <- Sys.getenv('TERM_PROGRAM_VERSION', NA)
    if(!is.na(version) && as.numeric_version(version) < '3.2.8'){
      packageStartupMessage("Your iTerm2 is outdated which may crash R. Upgrade ASAP")
    }
  }
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
