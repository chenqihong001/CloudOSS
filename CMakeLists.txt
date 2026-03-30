cmake_minimum_required (VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)
set(FLUENTUI_BUILD_EXAMPLES OFF)


project (CloudOSS)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)
find_package(AWSSDK CONFIG COMPONENTS core dynamodb kinesis s3 REQUIRED)


qt_standard_project_setup(REQUIRES 6.8)


qt_add_executable(${PROJECT_NAME} 
    "src/cpp/main.cpp"
    "src/cpp/common/HttpClient.h"
    "src/cpp/common/HttpClient.cpp"
)


set(QML_SINGLETON_FILES
    
)

set(QML_FILES
    "src/qml/Main.qml"
)

set(CPP_FILES
    "src/cpp/services/AuthService.cpp"
    "src/cpp/services/AuthService.h"
)





set_source_files_properties(${QML_SINGLETON_FILES}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)


qt_add_qml_module(${PROJECT_NAME} URI CloudOSS
    QML_FILES
    ${QML_FILES}
    ${QML_SINGLETON_FILES}
    SOURCES 
    ${CPP_FILES}
    RESOURCES
    "asserts/icons.qrc"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::Network
)
target_include_directories(${PROJECT_NAME} PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/common")
target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/services")
target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/models")



include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
