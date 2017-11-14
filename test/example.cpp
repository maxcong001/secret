#include "stdio.h"
#include "li/serviceDiscovery.hpp"
#include "li/connInterface.hpp"
#include "li/transConn.hpp"
#include "li/util.hpp"
#include "li/translibConn.hpp"
#include <iostream> 
#include "logger/logger.hpp"
void test_translibConn_connect()
{
	set_log_level(logger_iface::log_level::debug);
	auto conn_p = translibConn<void *, SRVC_INFO>::instance();
	conn_p->init();
	//LI_RETURN connect(connInd ind, connInfo info)
	translib::TcpSocket *ind;
	SRVC_INFO info;
	info.ip = LI_IP;
	info.port = std::to_string(LI_PORT);
	conn_p->connect((void **)&ind, info);

	while (1)
	{
		/*
		getchar();

		std::string message("test");
		printf("now send the message!\n");
		conn_p->send(ind, message.c_str(), message.size());
		printf("send out the message!\n");
		*/

		std::string tmp;
		__LOG(debug, "type the message to send");
		std::cin >>tmp;
		conn_p->send(ind, tmp.c_str(), tmp.size());
	}
}

int main()
{
	test_translibConn_connect();
	printf("hello world!");
}
