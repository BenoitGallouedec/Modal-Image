CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET("OpenCV_DIR" "D:/Developpement/OpenCV/build")
FIND_PACKAGE(OpenCV REQUIRED)

PROJECT(Projet)

#ADD_EXECUTABLE(test1 test.cpp)
ADD_EXECUTABLE(gregorianReader gregorianReader.cpp image.h test.h test.cpp lineDetector.h lineDetector.cpp segmentation.h segmentation.cpp)

TARGET_LINK_LIBRARIES(gregorianReader ${OpenCV_LIBS})