function (AddUnitTestsDirectory DIRECTORY)
    if(${ENABLE_TESTING})
        add_subdirectory( ${DIRECTORY} )
    endif()
endfunction(AddUnitTestsDirectory)

function (AddUnitTest PROJECT_NAME SRC)

    find_package(GTest REQUIRED)

    add_executable(${PROJECT_NAME}
        ${SRC}
    )

   add_test(NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} 
   )
   
   target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   )

   target_link_libraries(${PROJECT_NAME}
        PUBLIC
            GTest::GTest
            GTest::Main
            gmock
   )

   DefaultInstall(${PROJECT_NAME})

endfunction(AddUnitTest)