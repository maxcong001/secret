#pragma once
#include "li/util.hpp"
#include "logger/logger.hpp"
#include "li/connManager.hpp"
#include "li/translibcs.hpp"
#include <algorithm>
#include "translib/tcpServerTPKT.h"
#include "translib/tcpClientTPKT.h"
#include <stdlib.h>
#include <map>
#include <arpa/inet.h>
#define LI_CLIENT ture
#define LI_SERVER false

// only support one server, multi client
template <typename connInd, typename connInfo>
class translibConn // : public connInterface<connInd, connInfo>
{
  public:
    translibConn(){};
    ~translibConn()
    {
        if (!server_p)
        {
            delete server_p;
            server_p = NULL;
        }
    };
    /*-------------------------------------------------------------------------------------*/
    // child class special, this is singleton, as translib play a role of llc management
    typedef connInd conn_ind_t;
    typedef connInfo conn_info_t;

    static translibConn<connInd, connInfo> *instance()
    {
        static translibConn<connInd, connInfo> *ins = new translibConn<connInd, connInfo>();
        return ins;
    }
    LI_RETURN init()
    {
        conn_ind_t _ind;
        listen(&_ind);
    }

    LI_RETURN send(connInd ind, const char *msg, length_t length)
    {
        // note: here we can use usr memory and let user reserve 4 byte in the header, but this is a burden to user
        // here we choose to send the header first and then send the message.
        char tpkt_header[TPKT1_LENGTH];

        tpkt_header[0] = TPKT_VERSION1_LOW_BYTE;
        tpkt_header[1] = ((length + TPKT1_LENGTH) & 0xFFFFFF) >> 16U;
        tpkt_header[2] = ((length + TPKT1_LENGTH) & 0xFFFF) >> 8U;
        tpkt_header[3] = ((length + TPKT1_LENGTH) & 0xFF);
        //__LOG(debug, "[translibConn] now send the header");
        bool ret = ((translib::TcpSocket *)ind)->send(tpkt_header, sizeof(tpkt_header));
        if (!ret)
        {
            __LOG(error, "[translibConn] send TPKT header fail!");
            return LI_FAIL;
        }
        //__LOG(debug, "[translibConn] now send the body");
        return (((translib::TcpSocket *)ind)->send(msg, length)) ? LI_SUCCESS : LI_FAIL;
    }

    LI_RETURN close(connInd ind)
    {
        // to do
        // here may have some issue
        // later we will call session.stop and 
        (translib::TcpSocket *)ind->close();
    }
    LI_RETURN connect(connInd *ind, connInfo info)
    {
        liClient *client_p = new liClient(loop);
        if (loop.status() != translib::Loop::StatusRunning)
        {
            loop.start();
        }
        *ind = (connInd)client_p;
        // to do check if atoi return success
        return (client_p->connect(info.ip.c_str(), atoi(info.port.c_str()))) ? LI_SUCCESS : LI_FAIL;
    }
    LI_RETURN listen(connInd *ind_ptr) //, TOKEN _token, connInfo info)
    {
        __LOG(debug, "");
        server_p = new liServer();
        ind_ptr = (void **)&server_p;

        //        OSENDPOINT remote_ep;
        //      getLocalOSEP(&remote_ep);
        std::string _IP(LI_IP);
        //    OSEP2IP(&remote_ep, _IP);
        // to do check if atoi return success
        //return (server_p->listen(ind_ptr, _IP)) ? LI_SUCCESS : LI_FAIL;
        return (server_p->listen(_IP.c_str(), LI_PORT)) ? LI_SUCCESS : LI_FAIL;
    }

    //call manager    LI_RETURN onReceive(li_conn_ind_t ind, char *msg, length_t len);
    /*
    1. on receive
    2. on connect
    3. on new conneciton
    4. on connection closed
    */

    // this is for FE info update
    // void updateIDIndMap(std::string ID, conn_ind_t ind){};
    // void deleteIDIndMap(std::string ID, conn_ind_t ind){};

    bool getConnInfo(void *ind)
    {
        return indMap[ind];
    }
    void updateConnInfo(void *ind, bool cs)
    {
        indMap[ind] = cs;
    }

  private:
    translib::Loop loop;
    // here we need a map to save the ind<->type(server/client)
    // server 1, client 0
    std::map<void *, bool> indMap;

    // here we get things easy, only support one server and delete it when distructure
    liServer *server_p;
};
