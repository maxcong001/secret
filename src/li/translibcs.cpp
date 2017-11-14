#include "li/translibConn.hpp"
#include "li/translibcs.hpp"

void liClient::onConnected(int errorr)
{
    sockaddr_in dest;
    getAddr(&dest, sizeof(sockaddr_in));
    std::string tmp_IP(inet_ntoa(dest.sin_addr));

    std::string id;
    getIDWithIP(tmp_IP, id);
    connManager<translibConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(id, (void *)this);
    __LOG(debug, "[liClient] connected successfully");

    // this is for test
    std::string tmp_string("hello from client");
    sendWithTPKT(tmp_string.c_str(), tmp_string.size());
}

void liServer::onNewSession(translib::TcpSession *session)
{
    sockaddr_in dest;
    session->getAddr(&dest, sizeof(sockaddr_in));
    std::string tmp_IP(inet_ntoa(dest.sin_addr));
    __LOG(debug, "[liServer] tmp_IP is " << tmp_IP);
    if (indIsIC(tmp_IP))
    {
        // find the ind
        __LOG(debug, "the connection is form IC");
        std::string id;

        if (getIDWithIP(tmp_IP, id))
        {
            connManager<translibConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(id, (void *)this);
        }
        else
        {
            __LOG(error, "get ID fail!");
        }
    }
    else
    {
        // did not find the ind build up conn info and insert it to service discovery class
        SRVC_INFO info;
        info.ip = tmp_IP;
        // note: port is not useful, do not set it here
        info.srvcType = SRVC_TYPE::SRVC_INFO_FE;
        //info.insId = "FE_" + tmp_IP;
        info.insId = (void *)this;
        ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
        std::list<SRVC_INFO> tmp_list;
        tmp_list.push_back(info);
        ins->setSrvcInfo(SRVC_TYPE::SRVC_INFO_FE, tmp_list);
        connManager<translibConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(info.insId, (void *)this);
    }

    __LOG(debug, "[liClient] connected successfully");
}
bool getIDWithInd(void *ind, std::string &id)
{
    sockaddr_in dest;
    ((translib::TcpSocket *)ind)->getAddr(&dest, sizeof(sockaddr_in));
    std::string tmp_ip(inet_ntoa(dest.sin_addr));

    return getIDWithIP(tmp_ip, id);
}
bool getIDWithIP(std::string IP, std::string &id)
{

    ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
    std::list<SRVC_INFO> tmp_list = ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_SPFE);
    tmp_list.merge(ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_PROXY));
    tmp_list.merge(ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_IC));

    for (std::list<SRVC_INFO>::iterator it = tmp_list.begin(); it != tmp_list.end(); it++)
    {
        if ((it->ip).compare(IP) == 0)
        {
            id = it->insId;
            return true;
        }
        else
        {
            // did not find the IP
        }
    }
    return false;
}