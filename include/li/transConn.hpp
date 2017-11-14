#pragma once
#include "li/util.hpp"
#include "logger/logger.hpp"
#include "li/connManager.hpp"
#include "li/translibCb.hpp"
#include "li/connManager.hpp"
#include <algorithm>

#include "boost/variant.hpp"
//boost::variant<DW_MSG_DATA_TYPES> msg_data;
//DwDNSRspData::ptr_t dnsPtr = boost::get<DwDNSRspData::ptr_t>(msg->msg_data);
// there will be five classes,
/***********************************************************************************************************************/
// this is for client proxy
class liClientProxy : public boost::enable_shared_from_this<liClientProxy>
{
};

/***********************************************************************************************************************/
// this is for client IC
class liClientIC : public boost::enable_shared_from_this<liClientIC>
{
};

/***********************************************************************************************************************/
// this is for client SPFE
class liClientSPFE : public boost::enable_shared_from_this<liClientSPFE>
{
};
/***********************************************************************************************************************/
// this is for server IC
class liServerIC : public boost::enable_shared_from_this<liServerIC>
{
};
/***********************************************************************************************************************/
// this is for server proxy

class liServerProxy : public boost::enable_shared_from_this<liServerProxy>
{
};
#if 0
// for translib
// the connInfo should be SRVC_INFO
// TOKEN is void *
// connInd is void *
/*
std::string IP;
bool withIP(SRVC_INFO info)
{
    return ((info.ip).compare(IP) == 0) ? 1 : 0;
}
*/
template <typename connInd, typename connInfo>
class transConn // : public connInterface<connInd, connInfo>
{
  public:
    transConn(){};
    ~transConn(){};
    /*-------------------------------------------------------------------------------------*/
    // child class special, this is singleton, as translib play a role of llc management
    typedef connInd conn_ind_t;
    typedef connInfo conn_info_t;

    static transConn<connInd, connInfo> *instance()
    {
        static transConn<connInd, connInfo> *ins = new transConn<connInd, connInfo>();
        return ins;
    }

    /*-------------------------------------------------------------------------------------*/
    // common
    LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len)
    {
#ifdef TRANS
        int ret = TransSend(ind, _token, msg, len);
        if (ret >= TRANS_SUCCESS)
        {
            return LI_SUCCESS;
        }
#endif
    }
    LI_RETURN send(connInd ind, void *msg, length_t len)
    {
        return send(ind, NULL, msg, len);
    }
    LI_RETURN close(connInd ind, TOKEN _token = NULL) {} // = NULL){};

    /*-------------------------------------------------------------------------------------*/
    // as server
    LI_RETURN listen(connInd *ind_ptr) //, TOKEN _token, connInfo info_local)
    {
#ifdef TRANS
        //build localEP
        OSENDPOINT localEP;
        /*
        if (info2OSEP(info_local, &localEP))
        {
        }
        else
        {
            return LI_FAIL;
        }
        */
        getLocalOSEP(&localEP);
        OSEP_MKIPTCP(&localEP, LI_PORT);
        TransSockOpts opt;
        TransEventT event;
        // build trans event
        if (buildTransEvent(event) == LI_SUCCES)
        {
            int ret;
            // build the trans option
            if (buildTransOption(opt) == LI_SUCCES)
            {
            }
            else
            {
                return LI_FAIL;
            }
            // now we have trans event and trans option
            /*
                int TransListen(
                OUT void **newLLC,
                IN void *newULC,
                IN void *associatedLLC,
                IN OSENDPOINT *localEP,
                IN PTransEventT cb,
                IN TransSockOpts *opts);
            */
            int ret = TransListen(ind_ptr, _token, NULL, &localEP, event, &opt);
            if (TRANS_SUCCESS == ret)
            {
                return LI_SUCCESS;
            }
            else
            {
                return LI_FAIL;
            }
        }
        else
        {
            return LI_FAIL;
        }
#endif
    }

    /*-------------------------------------------------------------------------------------*/
    // as client
    LI_RETURN connect(connInd *ind_ptr, connInfo info_remote)
    {
        return connect(ind_ptr, NULL, info_remote);
    }
    LI_RETURN connect(connInd *ind_ptr, TOKEN _token, connInfo info_remote)
    {
#ifdef TRANS
        //build localEP,remoteEP
        OSENDPOINT localEP;
        OSENDPOINT remoteEP;
        if (info2OSEP(info_remote, &remoteEP))
        {
        }
        else
        {
            return LI_FAIL;
        }
        if (!getLocalOSEP(&localEP))
        {
            // get local endpoint fail
            return LI_FAIL;
        }

        TransSockOpts opt;
        TransEventT event;
        // build trans event
        if (buildTransEvent(event) == LI_SUCCES)
        {
            int ret;
            // note: if the connection is connect to proxy, we should not enable TPKT
            // build the trans option
            if (info_remote.type == SRVC_INFO::SRVC_INFO_PROXY)
            {
                if (buildTransOptionNoTPKT(opt) == LI_SUCCES)
                {
                }
                else
                {
                    return LI_FAIL;
                }
            }
            else
            {
                if (buildTransOption(opt) == LI_SUCCES)
                {
                }
                else
                {
                    return LI_FAIL;
                }
            }

            // now we have trans event and trans option
            int ret = TransConnect(ind_ptr, _token, NULL, remoteEP, localEP, event, &opt, LI_TRANS_CONNECT_TIMEOUT);
            switch (ret)
            {
            case TRANS_SUCCESS:
                /* Get connection accept result from peer
                ** This usually does not happen with a non-blocking socket unless the
                ** peer accepts the connection at the same time when TransConnect() called
                */
                // do the same work as LI_onConnectedCb
                LI_onConnectedCb(*ind_ptr, _token);
            // fall through
            case TRANS_PENDING:
                // connection, do nothing, when the connection established, callback will handld the work
                return LI_SUCCESS;
            //break;
            default:
                // Get immediate connection failure result from peer. This usually does not
                // happen with a non-blocking socket unless the peer rejects the connection
                // at the same time when TransConnect() issues the connect request.
                // For example, if the remote IP is 127.0.0.1 (localhost), it could run
                // into this situation : there is no network payload exchanged, all connect
                // request and connect rejection are done internally
                // Do the same work as onConnectErrorNotifyCB( void *llc, void *ulc, int errorNum)
                LI_onConnErrorCb(*ind_ptr, _token, 0);
                return LI_FAIL;
                //break;
            }
        }
        else
        {
            return LI_FAIL;
        }
#endif
    }

    // callback def
    int onReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len)
    {
        __LOG(debug, "");
        connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->onReceiveCb(ind, msg, len);
    };

    int onConnectedCb(connInd ind, TOKEN _token)
    {
        std::string tmp_IP;
        if (getIDWithInd(ind, tmp_IP))
        {
            connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(tmp_IP, ind);
        }
        else
        {
            // did not find the ind
        }
    };
    int onConnClosedCb(connInd ind, TOKEN _token, int errorNumber)
    {
        std::string tmp_IP;
        if (getIDWithInd(ind, tmp_IP))
        {
            connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->deleteIDIndMap(tmp_IP, ind);
        }
        else
        {
            // did not find the ind
        }
    }
    int onConnErrorCb(connInd ind, TOKEN _token, int errorNumber)
    {

        // delete the info(not needed, as we did not connect successfully)
        // to do  start a timer
        // reconnect

        // close the connection
        TransClose(ind, NULL);
    }
    int onSocketErrorCb(connInd ind, TOKEN _token, int errorNumber)
    {
        // to do
    }
    int onListenErrorCb(connInd ind, TOKEN _token, int errorNuber)
    {
        // to do
    }
    int onNewConnCb(connInd ind, TOKEN old_token, TOKEN *new_token, connInfo &info)
    {
        std::string tmp_IP;
        if (indIsIC(ind, tmp_IP))
        {
            // find the ind
            // should not happen
        }
        else
        {
            // did not find the ind build up conn info and insert it to service discovery class
            SRVC_INFO info;
            info.ip = tmp_IP;
            // note: port is not useful, do not set it here
            info.srvcType = SRVC_TYPE::SRVC_INFO_FE;
            info.insId = "FE_" + tmp_IP;

            ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
            std::list<connInfo> tmp_list;
            tmp_list.push_back(info);
            ins->setSrvcInfo(SRVC_TYPE::SRVC_INFO_FE, tmp_list);
        }
    }

  private:
    bool indIsIC(connInd ind, std::string &ip)
    {
#ifdef TRANS
        // get osep from ind
        OSENDPOINT osep;
        TransGetConnectionAddresses(ind, NULL, &osep, NULL);

        // get ip form osep
        std::string tmp_ip;
        OSEP2IP(&osep, tmp_ip);

        //ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
        std::list<SRVC_INFO> tmp_list = ICSrvcInfo<SRVC_INFO>::instance()->getSrvcInfo(SRVC_TYPE::SRVC_INFO_IC);

        for (std::list<SRVC_INFO>::iterator it = tmp_list_list.begin(); it != tmp_list.end(); it++)
        {
            if ((it->ip).compare(tmp_ip) == 0)
            {
                ip = tmp_ip;
                return true;
            }
            else
            {
                // did not find the IP
            }
        }
        return false;
#endif
    }
    bool getIDWithInd(connInd ind, std::string &ip)
    {
#ifdef TRANS
        // get osep from ind
        OSENDPOINT osep;
        TransGetConnectionAddresses(ind, NULL, &osep, NULL);

        // get ip form osep
        std::string tmp_ip;
        OSEP2IP(&osep, tmp_ip);

        ICSrvcInfo<SRVC_INFO> *ins = ICSrvcInfo<SRVC_INFO>::instance();
        std::list<SRVC_INFO> tmp_list = ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_SPFE);
        tmp_list.merge(ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_PROXY));
        tmp_list.merge(ins->getSrvcInfo(SRVC_TYPE::SRVC_INFO_IC));

        /*std::list<SRVC_INFO>::iterator it = std::find_if(tmp_list_list.begin(), tmp_list.end(), withIP);
        if (it != tmp_list.end())
        {
            connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(IP, ind);
        }
        else
        {
            std::cout << "did not find the IP info" << std::endl;
        }*/
        for (std::list<SRVC_INFO>::iterator it = tmp_list_list.begin(); it != tmp_list.end(); it++)
        {
            if ((it->ip).compare(tmp_ip) == 0)
            {
                ip = it->insId;
                return true;
                //connManager<transConn<void *, SRVC_INFO>, ICSrvcInfo<SRVC_INFO>>::instance()->updateIDIndMap(tmp_ip, ind);
            }
            else
            {
                // did not find the IP
            }
        }
        return false;
#endif
    }
};

#ifdef TRANS
int LI_onReceiveCb(void *llc, void *ulc, void *msg, length_t len)
{
    transConn<void *, SRVC_INFO>::instance()->onReceiveCb(llc, ulc, msg, len);
}
int LI_onConnectedCb(void *llc, void *ulc)
{
    transConn<void *, SRVC_INFO>::instance()->onReceiveCb(llc, ulc);
}
int LI_onConnClosedCb(void *llc, void *ulc, int errNub)
{
    transConn<void *, SRVC_INFO>::instance()->onConnClosedCb(llc, ulc, errNub);
}
int LI_onConnErrorCb(void *llc, void *ulc, int errNub)
{
    transConn<void *, SRVC_INFO>::instance()->onConnErrorCb(llc, ulc, errNub);
}
int LI_onSocketErrorCb(void *llc, void *ulc, int errNub)
{
    transConn<void *, SRVC_INFO>::instance()->onSocketErrorCb(llc, ulc, errNub);
}
int LI_onNewConnCb(void *llc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddrn)
{
    SRVC_INFO info;
    // need to convert the osendpoint to conn info

    if (OSEP2info(remoteAddrn, info))
    {
        transConn<void *, SRVC_INFO>::instance()->onNewConnCb(llc, listeningUlc, returnedUlc, info);
    }
}
int LI_onListenErrorCb(void *llc, void *ulc, int errNub)
{
    transConn<void *, SRVC_INFO>::instance()->onListenErrorCb(llc, ulc, errNub);
}
#ifdef TRANS
// note: here is the callbacks without tpkt
LI_RETURN buildTransEvent(TransEventT &LI_trans_event)
{
    // set tranlib call back
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyPacketArrived = LI_onReceiveCb;
    LI_trans_event.TransNotifyNewConnection = LI_onNewConnCb;
    LI_trans_event.TransNotifyConnected = LI_onConnectedCb;
    LI_trans_event.TransNotifyConnectionClosed = LI_onConnClosedCb;
    LI_trans_event.TransNotifyConnectError = LI_onConnErrorCb;
    LI_trans_event.TransNotifyListenError = LI_onListenErrorCb;
    // The TransNotifyTimeout call back function is required if and only if the
    // Translib user want to use TransSetTimerMS() API to set a timer. As
    // IM always uses TransSetTimerDirectMS()
    // to set timer, there is no need to provide such a call back function here.
    LI_trans_event.TransNotifyTimeout = NULL;
    LI_trans_event.TransNotifySocketError = LI_onSocketErrorCb;
    LI_trans_event.TransNotifyUDPPacketArrived = NULL;
}
// options with TPKT
LI_RETURN buildTransOption(TransSockOpts &options)
{
    memset(&options, 0, sizeof(TransSockOpts));
    options.flags =
        TRANS_FLAG_LOCAL_PORT |
        TRANS_FLAG_NON_BLOCKING |
        TRANS_FLAG_SP_TPKT |
        TRANS_FLAG_SP_TPKT_V1 |
        // TRANS_USE_PAYLOAD_BUF_FOR_SNDTPKT |
        TRANS_FLAG_BULK_READ |
        // need to support send while receiving disable this flag
        // TRANS_FLAG_SAFE_ARRIVED |
        TRANS_FLAG_REUSEADDR |
        TRANS_FLAG_LOWER_LOG |
        //The TransNotifySocketError() callback is used instead of transClose().
        //TRANS_FLAG_NOCLOSE_ON_ERROR |
        TRANS_FLAG_SOCKET_SNDBUF |
        TRANS_FLAG_SOCKET_RCVBUF |
        TRANS_FLAG_TCP |
        TRANS_FLAG_SOCKET_LINGER;

    options.send_buf_size = LI_TRANS_SEND_BUFFER_SIZE;
    options.recv_buf_size = LI_TRANS_RECV_BUFFER_SIZE;
}
LI_RETURN buildTransOptionNoTPKT(TransSockOpts &options)
{
    memset(&options, 0, sizeof(TransSockOpts));
    options.flags =
        TRANS_FLAG_LOCAL_PORT |
        TRANS_FLAG_NON_BLOCKING |
        //        TRANS_FLAG_SP_TPKT |
        //        TRANS_FLAG_SP_TPKT_V1 |
        //        TRANS_USE_PAYLOAD_BUF_FOR_SNDTPKT |
        TRANS_FLAG_BULK_READ |
        // need to support send while receiving disable this flag
        // TRANS_FLAG_SAFE_ARRIVED |
        TRANS_FLAG_REUSEADDR |
        TRANS_FLAG_LOWER_LOG |
        //The TransNotifySocketError() callback is used instead of transClose().
        //TRANS_FLAG_NOCLOSE_ON_ERROR |
        TRANS_FLAG_SOCKET_SNDBUF |
        TRANS_FLAG_SOCKET_RCVBUF |
        TRANS_FLAG_TCP |
        TRANS_FLAG_SOCKET_LINGER;

    options.send_buf_size = LI_TRANS_SEND_BUFFER_SIZE;
    options.recv_buf_size = LI_TRANS_RECV_BUFFER_SIZE;
}
#endif
#endif
#if 0
// deleted version two
template <typename connInd, typename connInfo>
class transConnClient : public connInterface_client<connInd, connInfo>
{
  public:
    typedef connInd conn_ind_t;
    typedef connInfo conn_info_t;

    transConnClient(){};
     ~transConnClient()
    {
        close(client_ind);
    };
    /*-------------------------------------------------------------------------------------*/
    // from interface.
     LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len){};
     LI_RETURN close(connInd ind){};
     LI_RETURN connect(connInd &ind, connInfo info){};

    // client callback def
     int onClientReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len){};
     int onClinetConnectedCb(connInd ind, TOKEN _token){};
     int OnClientConnClosedCb(connInd ind, TOKEN _token, int errorNumber){};
     int onClientConnErrorCb(connInd ind, TOKEN _token, int errorNumber){};
     int onClientSocketErrorCb(connInd ind, TOKEN _token, int errorNumber){};

    /*-------------------------------------------------------------------------------------*/
    // class special
    static transConnClient<connInd, connInfo> *instance()
    {
        static transConnClient<connInd, connInfo> *ins = new transConnClient<connInd, connInfo>();
        return ins;
    }
};

int clientOnReceived(void *llc, void *ulc, char *msg, int size)
{
}

int clientOnConnected(void *llc, void *ulc)
{
}
int clientOnClosed(void *llc, void *ulc, int errorNum)
{
}

int clientOnConnectError(void *llc, void *ulc, int errorNum)
{
}

int clientOnSocketError(void *llc, void *ulc, int error)
{
}

class transConnServer : public connInterface_server<connInd, connInfo>
{
  public:
    typedef connInd conn_ind_t;
    typedef connInfo conn_info_t;

    transConnServer(){};
     ~transConnServer()
    {
        close(server_ind);
    };
    /*-------------------------------------------------------------------------------------*/

     LI_RETURN send(connInd server_ind, TOKEN _token, void *msg, length_t len){};
     LI_RETURN close(connInd ind){};
     LI_RETURN listen(connInd &ind, connInfo info){};

    // server callback def
     int onServerNewConnCb(connInd session_ind, TOKEN old_token, TOKEN *new_token, connInfo *info){};
     int OnServerReceiveCb(connInd session_ind, TOKEN _token, void *msg, length_t len){};
     int OnServerConnClosedCb(connInd session_ind, TOKEN _token, int errorNuber){};
     int onServerListenErrorCb(connInd session_ind, TOKEN _token, int errorNuber){};
     int OnServerSocketErrorCb(connInd session_ind, TOKEN _token, int errorNuber){};

    /*-------------------------------------------------------------------------------------*/
    // class special
    static transConnServer<connInd, connInfo> *instance()
    {
        static transConnServer<connInd, connInfo> *ins = new transConnServer<connInd, connInfo>();
        return ins;
    }
};
#endif
// deleted  version one
#if 0


typedef struct _sock_state
{
    struct _thread_info *pool_thread;
    void *ulc;
    //      unsigned int trans_flags;
    TransSockOpts sopts;
    char *recv_buffer;
    int recv_buffer_length;
    int state;
    int os_socket;
    int error;
    PTransEventT ctrl;
#if defined(__DEBUG__) || defined(_DEBUG) || defined(DEBUG) || defined(_DEBUG_)
    unsigned int stamp;
#endif
    struct sockaddr_in laddr;
    int udp_connected;
    int index;
    void *timer_llc;
} SockStateT, *PSockStateT;
PUBLIC
int TransConnect(
        OUT void**              llc,
        IN  void*               ulc,
        IN  void*               associatedLLC,
        IN  OSENDPOINT*         remoteEP,
        IN  OSENDPOINT*         localEP,
        IN  PTransEventT        cb,
        IN  PTransSockOpts        opts,
        IN  int                 timeout)


/*
    LI_trans_event.TransNotifyPacketArrived = IM_onReceived;
    LI_trans_event.TransNotifyNewConnection = IM_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = IM_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = IM_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = IM_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = IM_onListenErrorNotifyCB;
    // The TransNotifyTimeout call back function is required if and only if the
    // Translib user want to use TransSetTimerMS() API to set a timer. As
    // IM always uses TransSetTimerDirectMS()
    // to set timer, there is no need to provide such a call back function here.
    LI_trans_event.TransNotifyTimeout = NULL;
    LI_trans_event.TransNotifySocketError = IM_onSocketError;
    LI_trans_event.TransNotifyUDPPacketArrived = NULL;
    */

template <typename connInd, typename connInfo>
class transConnInterface : public connInterface<connInd, connInfo>
{
  public:
    typedef connInd conn_ind_t;
    typedef connInfo conn_info_t;
    transConnInterface()
    {
#ifdef translib
        // for client
        memset(&LI_trans_event, 0, sizeof(TransEventT));
        LI_trans_event.TransNotifyPacketArrived = NULL;
        LI_trans_event.TransNotifyNewConnection = NULL;
        LI_trans_event.TransNotifyConnected = NULL;
        LI_trans_event.TransNotifyConnectionClosed = NULL;
        LI_trans_event.TransNotifyConnectError = NULL;
        LI_trans_event.TransNotifyListenError = NULL;
        LI_trans_event.TransNotifyTimeout = NULL;
        LI_trans_event.TransNotifySocketError = NULL;
        LI_trans_event.TransNotifyUDPPacketArrived = NULL;

        // for server
        memset(&LI_trans_event, 0, sizeof(TransEventT));
        LI_trans_event.TransNotifyPacketArrived = NULL;
        LI_trans_event.TransNotifyNewConnection = NULL;
        LI_trans_event.TransNotifyConnected = NULL;
        LI_trans_event.TransNotifyConnectionClosed = NULL;
        LI_trans_event.TransNotifyConnectError = NULL;
        LI_trans_event.TransNotifyListenError = NULL;
        LI_trans_event.TransNotifyTimeout = NULL;
        LI_trans_event.TransNotifySocketError = NULL;
        LI_trans_event.TransNotifyUDPPacketArrived = NULL;
#endif
    };
     ~transConnInterface(){};
    static transConnInterface<connInd, connInfo> *instance()
    {
        __LOG(debug, "get trans middleware instance");
        static transConnInterface<connInd, connInfo> *ins = new transConnInterface<connInd, connInfo>();
        return ins;
    }
    /*-------------------------------------------------------------------------------------*/
    // common
    LI_RETURN send(connInd server_ind, TOKEN _token, void *msg, length_t len){};
    LI_RETURN close(connInd ind){};

    void test()
    {
        __LOG(debug, "test");
    }

    void test1()
    {
        __LOG(debug, "test1");
        (this->instance())->test();
    }
    /*-------------------------------------------------------------------------------------*/
    // as server
    LI_RETURN listen(connInd &ind, connInfo info){};
    // server callback def
    int onServerNewConnCb(connInd session_ind, TOKEN old_token, TOKEN *new_token, connInfo *info)
    {
        __LOG(debug, "onServerNewConnCb");
    };
    int OnServerReceiveCb(connInd session_ind, TOKEN _token, void *msg, length_t len)
    {
        __LOG(debug, "OnServerReceiveCb");
    };
    int OnServerConnClosedCb(connInd session_ind, TOKEN _token, int errorNuber){};
    int onServerListenErrorCb(connInd session_ind, TOKEN _token, int errorNuber){};
    int OnServerSocketErrorCb(connInd session_ind, TOKEN _token, int errorNuber){};

    /*-------------------------------------------------------------------------------------*/
    // as client
    LI_RETURN connect(connInd &ind, connInfo info){};
    // client callback def
    int onClientReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len){};
    int onClinetConnectedCb(connInd ind, TOKEN _token){};
    int OnClientConnClosedCb(connInd ind, TOKEN _token, int errorNumber){};
    int onClientConnErrorCb(connInd ind, TOKEN _token, int errorNumber){};
    int onClientSocketErrorCb(connInd ind, TOKEN _token, int errorNumber){};

/*-------------------------------------------------------------------------------------*/
// data
#ifdef translib
    // Contains callback functions of translib
    TransEventT li_trans_event_client;
    TransEventT li_trans_event_server;
    // Socket options of translib
    TransSockOpts li_trans_sock_opts;
#endif
};

#endif
#endif
