#include <glog/logging.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dcel/dcel.hh>
#include <voronoi/geometry.hh>

TEST(DCELCheckTest, UnitTestingFrameworkWorks)
{
	LOG(INFO) << "Starting DCEL check test.";
	dcel::DCEL<voronoi::Point, voronoi::Point, int> m;
	LOG(INFO) << "Finishing DCEL check test.";
}

