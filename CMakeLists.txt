cmake_minimum_required (VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project (VscodeCppDemo)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)
find_package(aws-cpp-sdk-core REQUIRED)


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
    # components
    "src/qml/components/LoginCard.qml"
    "src/qml/components/DanmuToast.qml"
)

set(CPP_Files
   "src/cpp/common/UserManager.h"
   "src/cpp/services/AuthService.h"
   "src/cpp/services/AuthService.cpp"
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

)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::Network
)
target_include_directories(${PROJECT_NAME} PRIVATE aws-cpp-sdk-core)
target_link_libraries(${PROJECT_NAME} PRIVATE aws-cpp-sdk-core)
target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/common")
target_include_directories(${PROJECT_NAME} PRIVATE "src/cpp/services")

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
