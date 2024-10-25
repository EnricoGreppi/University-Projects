#include "test_point.hpp"
#include"test_vertex.h"
#include "test_intersector.h"
#include "test_segment.h"
#include "test_polygon.h"
#include "test_cutter.h"
#include "test_smartCutter.h"
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
