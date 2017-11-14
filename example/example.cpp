
#include "logger/logger.hpp"

#include "li/serviceDiscovery.hpp"
#include "li/connInterface.hpp"
#include "li/transConn.hpp"
#include "li/util.hpp"

int main()
{
    set_log_level(logger_iface::log_level::debug);
    __LOG(error, "hello logger!"
                     << "this is error log");
    __LOG(warn, "hello logger!"
                    << "this is warn log");
    __LOG(info, "hello logger!"
                    << "this is info log");
    __LOG(debug, "hello logger!"
                     << "this is debug log");
    /*
    transconnInterface<TOKEN, SRVC_INFO> test;
    auto ins = test.instance();
    ins->test1();
    auto ins1 = transconnInterface<TOKEN, SRVC_INFO>::instance();
    ins1->test1();

    connManager<transconnInterface<TOKEN, SRVC_INFO>, connManager<SRVC_INFO>> manager_test;
    manager_test.test();*/

    connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>> manager_test;
    manager_test.test();
    SRVC_INFO connInfo;
    std::list<SRVC_INFO> test_list;

    manager_test.onSrvcInc(SRVC_TYPE::SRVC_INFO_SPFE, test_list);

        __LOG(debug, "_____________________________________________________");
    transConn<void *, SRVC_INFO>::instance()->onReceiveCb(NULL, NULL, NULL, 0);
    //connManager<SRVC_INFO>::instance();
}
