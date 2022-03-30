# Fills return_list variable with all .h* files from current dir recursively.
function(header_directories return_list)
  file(GLOB_RECURSE NEW_LIST "*.h*")
  set(dir_list "")
  foreach(file_path ${NEW_LIST})
    get_filename_component(DIR_PATH "${file_path}" PATH)
    set(dir_list ${dir_list} "${DIR_PATH}")
  endforeach()
  list(REMOVE_DUPLICATES dir_list)
  set(${return_list}
      "${dir_list}"
      PARENT_SCOPE)
endfunction()
