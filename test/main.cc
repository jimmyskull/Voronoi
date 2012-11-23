#include <glog/logging.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
	google::InitGoogleLogging("SanityCheckTest");
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	google::ShutdownGoogleLogging();
	return ret;
}

