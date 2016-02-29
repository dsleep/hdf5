cmake_minimum_required (VERSION 3.1.0)
PROJECT (HDF5_JAVA_TEST Java)

set (CMAKE_VERBOSE_MAKEFILE 1)

INCLUDE_DIRECTORIES (
    ${HDF5_JAVA_JNI_BINARY_DIR}
    ${HDF5_JAVA_HDF5_LIB_DIR}
)

set (HDF5_JAVA_TEST_SRCS
    TestH5.java
    TestH5E.java
    TestH5Edefault.java
    TestH5Eregister.java
    TestH5Fparams.java
    TestH5Fbasic.java
    TestH5F.java
    TestH5Gbasic.java
    TestH5G.java
    TestH5Giterate.java
    TestH5Sbasic.java
    TestH5S.java
    TestH5Tparams.java
    TestH5Tbasic.java
    TestH5T.java
    TestH5Dparams.java
    TestH5D.java
    TestH5Dplist.java
    TestH5Lparams.java
    TestH5Lbasic.java
    TestH5Lcreate.java
    TestH5R.java
    TestH5P.java
    TestH5PData.java
    TestH5Pfapl.java
    TestH5Pvirtual.java
    TestH5Plist.java
    TestH5A.java
    TestH5Oparams.java
    TestH5Obasic.java
    TestH5Ocreate.java
    TestH5Ocopy.java
    TestH5PL.java
    TestH5Z.java
    TestAll.java
)

file (WRITE ${PROJECT_BINARY_DIR}/Manifest.txt
"Main-Class: test.TestAll
"
)

set (CMAKE_JAVA_INCLUDE_PATH "${HDF5_JAVA_LIB_DIR}/junit.jar;${HDF5_JAVA_LIB_DIR}/hamcrest-core.jar;${HDF5_JAVA_JARS};${HDF5_JAVA_LOGGING_JAR};${HDF5_JAVA_LOGGING_SIMPLE_JAR}")

add_jar (${HDF5_JAVA_TEST_LIB_TARGET} MANIFEST ${PROJECT_BINARY_DIR}/Manifest.txt ${HDF5_JAVA_TEST_SRCS})

get_target_property (${HDF5_JAVA_TEST_LIB_TARGET}_JAR_FILE ${HDF5_JAVA_TEST_LIB_TARGET} JAR_FILE)
#install_jar (${HDF5_JAVA_TEST_LIB_TARGET} ${HJAVA_INSTALL_DATA_DIR}/tests tests)
#get_target_property (${HDF5_JAVA_TEST_LIB_TARGET}_CLASSPATH ${HDF5_JAVA_TEST_LIB_TARGET} CLASSDIR)

add_dependencies (${HDF5_JAVA_TEST_LIB_TARGET} ${HDF5_JAVA_HDF5_LIB_TARGET})
set_target_properties (${HDF5_JAVA_TEST_LIB_TARGET} PROPERTIES FOLDER test/java)

set (HDF_JAVA_TEST_FILES
      h5ex_g_iterate.hdf
      JUnit-interface.txt
)

foreach (h5_file ${HDF_JAVA_TEST_FILES})
  set (dest "${PROJECT_BINARY_DIR}/${h5_file}")
  #message (STATUS " Copying ${h5_file}")
  add_custom_command (
      TARGET     ${HDF5_JAVA_TEST_LIB_TARGET}
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${PROJECT_SOURCE_DIR}/${h5_file} ${dest}
  )
endforeach (h5_file ${HDF_JAVA_TEST_FILES})

if (WIN32)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
else (WIN32)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
endif (WIN32)

set (CMAKE_JAVA_CLASSPATH ".")
foreach (CMAKE_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH})
  set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_INCLUDE_PATH}")
endforeach (CMAKE_INCLUDE_PATH)
set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${${HDF5_JAVA_TEST_LIB_TARGET}_JAR_FILE}")
set (testfilter "OK (598 tests)")

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set (CMD_ARGS "-Dhdf.hdf5lib.H5.loadLibraryName=hdf5_java_debug;")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_test (
    NAME JUnit-interface
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_TESTER=${CMAKE_Java_RUNTIME};${CMAKE_Java_RUNTIME_FLAGS}"
        -D "TEST_CLASSPATH:STRING=${CMAKE_JAVA_CLASSPATH}"
        -D "TEST_ARGS:STRING=${CMD_ARGS}-ea;org.junit.runner.JUnitCore"
        -D "TEST_PROGRAM=test.TestAll"
        -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_OUTPUT_DIRECTORY}"
        -D "TEST_FOLDER=${HDF5_BINARY_DIR}/java/test"
        -D "TEST_OUTPUT=JUnit-interface.out"
#        -D "TEST_LOG_LEVEL=trace"
        -D "TEST_EXPECT=0"
        -D "TEST_MASK_ERROR=TRUE"
        -D "TEST_FILTER:STRING=${testfilter}"
        -D "TEST_REFERENCE=JUnit-interface.txt"
        -P "${HDF_RESOURCES_DIR}/jrunTest.cmake"
)