#pragma once
#include "translib/tcpServerTPKT.h"
#include "translib/tcpClientTPKT.h"
#include <arpa/inet.h>
#include <translib/loop.h>
bool getIDWithInd(void *ind, std::string &id);
bool getIDWithIP(std::string IP, std::string &id);
class liServer : public translib::TcpServerTPKT
{
  public:
    void onListenError() override
    {
        __LOG(debug, "");
    }

    void onRead(translib::TcpSession *session, const uint8_t *msg, uint32_t len) override
    {
        // please do not do too much work here!
        std::string tmp((char *)msg, (uint32_t)len);
        __LOG(debug, "[liServer] receive " << len << " byte message"
                                           << ". message body is " << tmp);
    }
    void onSessionDisconnected(translib::TcpSession *session) override
    {
        __LOG(debug, " ");
    }
    bool indIsIC(std::string IP)
    {
        ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
        std::list<SRVC_INFO> tmp_list = ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_IC);
        __LOG(debug, "[liServer] tmp_list.size() is " << tmp_list.size());

        for (std::list<SRVC_INFO>::iterator it = tmp_list.begin(); it != tmp_list.end(); it++)
        {
            if ((it->ip).compare(IP) == 0)
            {
                return true;
            }
            else
            {
                // did not find the IP
            }
        }
        return false;
    }
    void onNewSession(translib::TcpSession *session) override;
    //sendWithTPKT
    translib::TcpSocket *_ind;
};
class liClient : public translib::TcpClientTPKT
{
  public:
    liClient(const translib::Loop &loop) : translib::TcpClientTPKT(loop){};

  protected:
    void onRead(uint8_t *msg, uint32_t len)
    {
        // please do not do too much work here!
        __LOG(debug, "[liClient] receive " << len << " byte message");
    }

    void onDisconnected()
    {
        __LOG(error, "[liClient] disconnected");
    }
    void onConnected(int errorr) override;

  private:
};