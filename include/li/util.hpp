#pragma once
#include <string>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
typedef boost::lock_guard<boost::mutex> LI_LOCK;
//	boost::mutex timer_mutex;
//Lock lock(timer_mutex);
typedef void *TOKEN;
typedef int length_t;
typedef void *transConnInd;
#define NTAS_IC_SERVER_LISTEN_PORT      9494
#define NTAS_IC_PEER_LISTEN_PORT        9495

enum LI_RETURN
{
    LI_SUCCESS = 0,
    LI_FAIL,
    LI_RETURN_MAX
};
struct SRVC_TYPE
{
    enum type
    {
        SRVC_INFO_MIN,
        SRVC_INFO_IC,
        SRVC_INFO_SPFE,
        SRVC_INFO_TAFE,
        SRVC_INFO_SCFE,
        SRVC_INFO_FE,
        SRVC_INFO_PROXY,
        SRVC_INFO_MINE,
        SRVC_INFO_MAX
    };
    static std::string toString(const SRVC_TYPE::type _type)
    {
        return (_type > SRVC_INFO_MAX || _type < SRVC_INFO_MIN) ? "undefined type" : (const std::string[]){"SRVC_INFO_MIN", "SRVC_INFO_IC", "SRVC_INFO_SPFE", "SRVC_INFO_TAFE", "SRVC_INFO_SCFE", "SRVC_INFO_FE", "SRVC_INFO_PROXY", "SRVC_INFO_MINE", "SRVC_INFO_MAX"}[_type];
    }
};

struct SRVC_INFO
{
    typedef SRVC_TYPE::type srvcType_t;
    typedef void *ind_t;

    std::string ip;
    std::string port;
    srvcType_t srvcType;
    ind_t insId;

    bool operator<(const SRVC_INFO &rhs) { return ip.compare(rhs.ip); }
};
#define LI_PORT 4567
#define LI_IP "127.0.0.1"

#ifdef TRANS
#define LI_TRANS_SEND_BUFFER_SIZE (1024 * 1024)
#define LI_TRANS_RECV_BUFFER_SIZE (1024 * 1024)
#define LI_TRANS_CONNECT_TIMEOUT 500

bool info2OSEP(liConnInfo info, OSENDPOINT *osep)
{
    return IP2OSEP(info.IP, info.port, osep);
}
bool OSEP2IP(OSENDPOINT *osep, std::string &_IP)
{
    struct sockaddr_in antelope;
    // to do check return code
    OSEP_EP2SOCKADDR(osep, &antelope, sizeof(sockaddr_in));

    std::string IP(inet_ntoa(antelope));
    _IP = IP;
}
// not now only support IPv4
bool IP2OSEP(std::string IP, unsigned short port, OSENDPOINT &osep, bool IPv4 = true)
{
    OSEP_INIT(&osep);
    OSEP_MKIPTCP(&osep, port);
    OSIPADDRSETTYPE(OSEP_IPADDR(osep), OSIPV4); // ((IPv4) ? OSIPV4 : OSIPV6));
    struct sockaddr_in antelope;
    if (inet_aton(IP.c_str(), &antelope.sin_addr) != 0)
    {
        OSIPADDRSETRAW((OSEP_IPADDR(osep)), &antelope.sin_addr, AF_RAWIPV4SIZE);
        return true;
    }
    else
    {
        return false;
    }
}
bool getLocalOSEP(OSENDPOINT &local_ep)
{
    OSIPADDR *osipaddr_ptr = SVCGET_IPADDRV4("NETWORK_OAM");
    if (NULL == osipaddr_ptr)
    {
        // log needed
        return false;
    }
    in_addr_t oam_ip;
    OSIPADDR2IN4(osipaddr_ptr, &oam_ip);
    OSIPADDRSETTYPE(OSEP_IPADDR(local_ep), OSIPV4);
    OSIPADDRSETRAW(OSEP_IPADDR(local_ep), &oam_ip, sizeof(oam_ip));
}
typedef int (*liNotifyPacketArrived)(
    void *llc,
    void *ulc,
    char *buf,
    int len);

typedef int (*liNotifyNewConnection)(
    void *newLLC,
    void *listeningULC,
    void **returnedULC,
    OSENDPOINT *remoteEP);

typedef int (*liNotifyConnected)(
    void *llc,
    void *ulc);

typedef int (*liNotifyConnectionClosed)(
    void *llc,
    void *ulc,
    int error);

typedef int (*liNotifyConnectError)(
    void *llc,
    void *ulc,
    int error);

typedef int (*liNotifyListenError)(
    void *llc,
    void *ulc,
    int error);

typedef int (*liNotifySocketError)(
    void *llc,
    void *ulc,
    int error);

#endif

/*
enum indType
{
    X_SERVER,
    FE_SERVER,
    X_CLIENT,
    PROXY_CLIENT,
    SPFE_CLIENT
};
*/
/*
struct liConnInfo
{
    std::string IP;
    unsigned short port;
    conn_type type;
    //   bool IP_type; // v4 or v6
    std::string ins_id;
};
*/
/*
struct connAllInfo
{
    conn_type type;
    TOKEN _token;
#ifdef TRANS
    OSENDPOINT osep;
#endif
    liConnInfo connInfo;
};
*/

/*
enum conn_type
{
    LI_SPFE,
    LI_TAFE,
    LI_SCFE,
    LI_FES,
    X_CONTAINER,
    LI_PROXY,
    LI_MINE
};
*/
/*
struct conn_type
{
    enum type
    {
        LI_MIN = 0,
        LI_SPFE,
        LI_TAFE,
        LI_SCFE,
        LI_FES,
        X_CONTAINER,
        LI_PROXY,
        LI_MINE,
        LI_MAX
    };
    static std::string toString(const conn_type::type _type)
    {
        return (_type > LI_MIN || _type < LI_MAX) ? "undefined type" : (const std::string[]){
                                                                           "LI_MIN",
                                                                           "LI_SPFE",
                                                                           "LI_TAFE",
                                                                           "LI_SCFE",
                                                                           "LI_FES",
                                                                           "X_CONTAINER",
                                                                           "LI_PROXY",
                                                                           "LI_MINE",
                                                                       }[_type];
    }
};
*/
