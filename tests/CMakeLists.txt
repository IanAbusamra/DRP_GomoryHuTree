add_executable(gomoryhu_tests
    test_gh_tree.cpp
)

target_link_libraries(gomoryhu_tests
    PRIVATE
      gomoryhu
      GTest::gtest
      GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gomoryhu_tests)