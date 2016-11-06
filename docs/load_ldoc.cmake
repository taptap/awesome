# To avoid copy pasting, some documentation is stored in reusable files
set(SHAPE_FILE "${SOURCE_DIR}/docs/common/${SHAPE_NAME}.lua")

set(path "${SOURCE_DIR}/docs/common/")

# Get the documentation file list
file(GLOB doc_files RELATIVE "${path}" "${path}/*.ldoc")

foreach(doc_file_name ${doc_files})
    # Read the file
    file(READ "${path}/${doc_file_name}" doc_file_content)

    # Remove the file extension
    string(REGEX REPLACE "\\.ldoc" "" DOC_FILE_NAME ${doc_file_name})

    # Create a new variable usable from lua files
    set(DOC_${DOC_FILE_NAME}_COMMON "Imported documentation\n\n${doc_file_content}")
endforeach()
