#include <muduo/utils/ConfFileRead.h>
#include <iostream>

using namespace muduo;

int main()
{
	ConfFileRead conf("/iridium/config/iridiumlog_muduo.conf");
	
	std::cout << conf.GetLevel() << std::endl;
	std::cout << conf.GetFileSize() << std::endl;
	
	return 0;
}