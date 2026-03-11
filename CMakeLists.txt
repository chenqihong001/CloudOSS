cmake_minimum_required (VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

project (CloudOSS)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)
find_package(AWSSDK CONFIG COMPONENTS core dynamodb kinesis s3 REQUIRED)
find_package(spdlog CONFIG REQUIRED)


qt_standard_project_setup(REQUIRES 6.8)


qt_add_executable(${PROJECT_NAME} 
    "src/cpp/main.cpp"
    "src/cpp/common/HttpClient.h"
    "src/cpp/common/HttpClient.cpp"
)


set(QML_Singleton_Files
     "src/qml/components/ToastService.qml"
)

set(QML_Files 
    "src/qml/Main.qml"
    "src/qml/pages/LoginPage.qml"
    "src/qml/pages/RegisterPage.qml"
    "src/qml/pages/dashboard/DashboardPage.qml"
    "src/qml/pages/dashboard/OverviewPage.qml"
    "src/qml/pages/dashboard/BucketManagerPage.qml"
    "src/qml/pages/dashboard/UploadManagerPage.qml"
    "src/qml/pages/dashboard/AnalyticsPage.qml"
    "src/qml/pages/dashboard/AccessControlPage.qml"
    "src/qml/pages/dashboard/LogManagerPage.qml"
    "src/qml/pages/dashboard/SettingsPage.qml"
    # components
    "src/qml/components/UiTheme.qml"
    "src/qml/components/UiCard.qml"
    "src/qml/components/UiButton.qml"
    "src/qml/components/UiInput.qml"
    "src/qml/components/UiBadge.qml"
    "src/qml/components/auth/AuthFrame.qml"
    "src/qml/components/DanmuToast.qml"
    "src/qml/components/GlowIcon.qml"


)

set(CPP_Files

   "src/cpp/services/AuthService.h"
   "src/cpp/services/AuthService.cpp"
   "src/cpp/common/ConfigManager.h"
   "src/cpp/common/ConfigManager.cpp"
   "src/cpp/common/AwsS3Client.h"
   "src/cpp/common/AwsS3Client.cpp"
)

set_source_files_properties(${QML_Singleton_Files}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)


qt_add_qml_module(${PROJECT_NAME} URI CloudOSS
    QML_FILES
    ${QML_Files}
    ${QML_Singleton_Files}
    SOURCES 
    ${CPP_Files}
    RESOURCES
    "resources/icons.qrc"

)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::Network
)
target_include_directories(${PROJECT_NAME} PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)


target_include_directories(${PROJECT_NAME} PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/common")
target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/services")



include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
