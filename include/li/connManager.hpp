#pragma once
#include "li/util.hpp"
#include "li/serviceDiscovery.hpp"
//#include "li/connInterface.hpp"
//#include "li/transConn.hpp"
#include <boost/bind.hpp>
#include <map>
#include <list>

template <typename serviceDiscovery>
class connManager
{
  public:
    // these conn info is same between client and server.

    typedef typename serviceDiscovery::conn_info_t li_conn_info_t;
    typedef typename li_conn_info_t::ind_t li_conn_ind_t;
    typedef typename serviceDiscovery::connList connList;
    typedef typename serviceDiscovery::serviceType serviceType;
    static connManager<serviceDiscovery> *instance()
    {
        static connManager<serviceDiscovery> *ins = new connManager<serviceDiscovery>();
        return ins;
    }
    LI_RETURN init()
    {
        // register callback to service discovery class
        serviceDiscovery::instance()->setOnSrvcIncCb(boost::bind(&connManager<serviceDiscovery>::onSrvcInc, this));
        serviceDiscovery::instance()->setOnSrvcDecCb(boost::bind(&connManager<serviceDiscovery>::onSrvcDec, this));
        // now listen on port for IC sync
        // #define NTAS_IC_SERVER_LISTEN_PORT      9494

        // now listen on port for FEs to connect
        // #define NTAS_IC_PEER_LISTEN_PORT        9495
    }

    void onSrvcInc(serviceType type, connList _list)
    {
    }

    void onSrvcDec(serviceType type, connList _list)
    {
    }
};
#if 0
template <typename connClass, typename serviceDiscovery>
class connManager
{
  public:
    // these conn info is same between client and server.
    typedef typename connClass::conn_ind_t li_conn_ind_t;
    typedef typename serviceDiscovery::conn_info_t li_conn_info_t;
    typedef typename serviceDiscovery::connList connList;
    typedef typename serviceDiscovery::serviceType serviceType;

    static connManager<connClass, serviceDiscovery> *instance()
    {
        static connManager<connClass, serviceDiscovery> *ins = new connManager<connClass, serviceDiscovery>();
        return ins;
    }

    LI_RETURN init()
    {
        // register callback to service discovery class
        // note: service discovery function should init later than connManager
        serviceDiscovery::instance()->setOnSrvcIncCb(boost::bind(&connManager<connClass, serviceDiscovery>::onSrvcInc, this));
        serviceDiscovery::instance()->setOnSrvcDecCb(boost::bind(&connManager<connClass, serviceDiscovery>::onSrvcDec, this));
        //    connClass::instance()->listen(&_ind);
        connClass::instance()->init();
    }

    LI_RETURN onReceive(li_conn_ind_t ind, char *msg, length_t len)
    {
        // app logic
    }
    LI_RETURN send(li_conn_ind_t ind, char *msg, length_t len)
    {
        return connClass::instance()->send(ind, msg, len);
    }

    /***********************************************************************************************************************************/
    void updateIDIndMap(std::string ID, li_conn_ind_t ind)
    {
        __LOG(debug, "updateIDIndMap");
        // need to lock here   to do
        id2ind_map[ID] = ind;
        ind2id_map[ind] = ID;
    }
    void deleteIDIndMap(std::string ID, li_conn_ind_t ind)
    {
        // need lock here  to do
        id2ind_map.erase(id2ind_map.find(ID));
        ind2id_map.erase(ind2id_map.find(ind));
    }
    void getInd(li_conn_ind_t &ind, std::string ID)
    {
        ind = id2ind_map[ID];
    }
    void getID(li_conn_ind_t ind, std::string &ID)
    {
        ID = ind2id_map[ind];
    }
    void onSrvcInc(serviceType type, connList _list)
    {
        if (type != SRVC_TYPE::SRVC_INFO_FE)
        {
            for (typename std::list<li_conn_info_t>::iterator it = _list.begin(); it != _list.end(); it++)
            {
                li_conn_ind_t ind;
                if (connClass::instance()->connect(&ind, *it) == LI_SUCCESS)
                {
                    // this should be called when onconnected
                    //updateIDIndMap((*it).insId, ind);
                }
                else
                {
                    // error log needed here
                    // connect fail
                }
            }
        }
        else
        {
            // note: this is FE, it is form conn, do nothing
        }
    }

    void onSrvcDec(serviceType type, connList _list)
    {
        if (type != SRVC_TYPE::SRVC_INFO_FE)
        {
            // need lock here  to do
            for (typename std::list<li_conn_info_t>::iterator it = _list.begin(); it != _list.end(); it++)
            {
                typename std::map<std::string, li_conn_ind_t>::iterator _it = id2ind_map.find((*it).insId);
                if (_it != id2ind_map.end())
                {
                    li_conn_ind_t ind = id2ind_map.find((*_it).insId);
                    if (connClass::instance()->close(ind) == LI_SUCCESS)
                    {
                        deleteIDIndMap((*it).insId, ind);
                    }
                    else
                    {
                        // error log needed here
                        // connect fail
                    }
                }
                else
                {
                    // did not find the IP in the list, something wrong with servicediscovry
                    // log here
                }
            }
        }
        else
        {
        }
    }
    void test()
    {
        __LOG(debug, "");
    }
    /*
    LI_RETURN send(std::string ID, char *msg, length_t len)
    {
        typename std::map<std::string, li_conn_ind_t>::iterator it = id2ind_map.find(ID);
        if (it != id2ind_map.end())
        {
            return connClass::instance()->send(it->second, msg, len);
        }
        else
        {
            // did not find the ind.
            return LI_FAIL;
        }
    }

    LI_RETURN onReceive(std::string ID, char *msg, length_t len)
    {
        // app logic
    }
    LI_RETURN send(li_conn_ind_t ind, char *msg, length_t len)
    {
        return connClass::instance()->send(ind, msg, len);
    }

    LI_RETURN onReceive(li_conn_ind_t ind, char *msg, length_t len)
    {
        typename std::map<li_conn_ind_t, std::string>::iterator it = ind2id_map.find(ind);
        // note: if we do not find the ind, we can do nothing
        // Maybe it is deleted by etcd
        return (it != ind2id_map.end()) ? (onReceive((it->second), msg, len)) : LI_FAIL;
    }
    */

    li_conn_ind_t _ind;
    std::map<std::string, li_conn_ind_t> id2ind_map;
    std::map<li_conn_ind_t, std::string> ind2id_map;
};


//deleted version
template <typename connClass, typename serviceDiscovery>
class connManager
{
  public:
    // these conn info is same between client and server.
    typedef typename connClass::conn_ind_t li_conn_ind_t;
    typedef typename connClass::conn_info_t li_conn_info_t;
    typedef typename serviceDiscovery::connList connList;

    static connManager<connClass, serviceDiscovery> *instance()
    {
        static connManager<connClass, serviceDiscovery> *ins = new connManager<connClass, serviceDiscovery>();
        return ins;
    }

    LI_RETURN close_connection(li_conn_ind_t ind, TOKEN _token);
    LI_RETURN send_to_peer(li_conn_ind_t ind, TOKEN _token, conn_type type, char *buf, int len)
    {
        switch (type)
        {
        case LI_SPFE:
            return send_to_spfe(ind, _token, buf, len);
        //   break;
        case LI_FES:
            return send_to_proxy(ind, _token, buf, len);
            break;
        case X_CONTAINER:
            return send_to_x(ind, _token, buf, len);
            break;
        case LI_PROXY:
            return send_to_fes(ind, _token, buf, len);
            break;
        default:
            break;
        }
    }
    LI_RETURN update_token(li_conn_ind_t ind, TOKEN _token);

    LI_RETURN send_to_spfe(li_conn_ind_t ind, TOKEN _token, char *buf, int len)
    {
        if (!ind)
        {
            // note: when the ind is NULL, we choose a connection and send message to it.
            if (spfe_ind.size())
            {
                // we need lock here to do later or change another rule to select connectin indicator
                //to do
                li_conn_ind_t tmp_ind = spfe_ind.pop_front();
                spfe_ind.push_back(tmp_ind);
                // update_token
                if (update_token(tmp_ind, _token) == LI_SUCCESS)
                {
                }
                else
                {
                    // update token fail //to do?
                    // error log
                }
                return connClass::instance()->send(tmp_ind, _token, buf, len);
            }
            else
            {
                // there is no spfe connection
                return LI_FAIL;
            }
        }
        else
        {
            // note: this could not happen so often. if the ind is not NULL, then we send message using this ind
            //     LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len){};
            // update_token
            if (update_token(ind, _token) == LI_SUCCESS)
            {
            }
            else
            {
                // update token fail //to do?
                // error log
            }
            return connClass::instance()->send(ind, _token, buf, len);
        }
    }
    LI_RETURN send_to_proxy(li_conn_ind_t ind, TOKEN _token, char *buf, int len)
    {
        if (!ind)
        {
            // note: when the ind is NULL, we choose a connection and send message to it.
            if (proxy_list.size())
            {
                // we need lock here later to do or change another rule to select connectin indicator
                //to do
                li_conn_ind_t tmp_ind = proxy_list.pop_front();
                proxy_list.push_back(tmp_ind);

                // update_token
                if (update_token(tmp_ind, _token) == LI_SUCCESS)
                {
                }
                else
                {
                    // update token fail //to do?
                    // error log
                }

                return connClass::instance()->send(tmp_ind, _token, buf, len);
            }
            else
            {
                // there is no spfe connection
                return LI_FAIL;
            }
        }
        else
        {
            // note: this could not happen so often. if the ind is not NULL, then we send message using this ind
            //     LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len){};
            // update_token
            if (update_token(ind, _token) == LI_SUCCESS)
            {
            }
            else
            {
                // update token fail //to do?
                // error log
            }
            return connClass::instance()->send(ind, _token, buf, len);
        }
    }
    LI_RETURN send_to_x(li_conn_ind_t ind, TOKEN _token, char *buf, int len)
    {
        // note: when this function is called. we need to send all the Xs.
        // need lock here, to do later
        if (x_list.size())
        {
            for (auto tmp : x_list)
            {
                if (connClass::instance()->send(ind, _token, buf, len) == LI_SUCCESS)
                {
                    // update_token
                    if (update_token(ind, _token) == LI_SUCCESS)
                    {
                    }
                    else
                    {
                        // update token fail //to do?
                        // error log
                    }
                }
                else
                {
                    // error log here
                    continue;
                }
            }
        }
        else
        {
            // x list is empty
            return LI_FAIL;
        }
    }
    LI_RETURN send_to_fes(li_conn_ind_t ind, TOKEN _token, char *buf, int len)
    {
        // note: unlike spfe or proxy, when send to fes, we need to specify one connection to send
        // so ind should not be NULL,
        // this ind should come form the token of proxy response message.
        if (ind)
        {
            // update_token 
            // this is not needed, as no response will return. just left here
            /*
            if (update_token(ind, _token) == LI_SUCCESS)
            {
            }
            else
            {
                // update token fail //to do?
                // error log
            }*/
            return connClass::instance()->send(ind, _token, buf, len);
        }
        else
        {
            // ind should not be NULL
            return LI_FAIL;
        }
    }

    LI_RETURN init()
    {
        //register callback to service discovery class
        // note: service discovery function should init later than connManager
        //(liServiceDiscovery<liConnInfo> * (liServiceDiscovery<liConnInfo>::instance()))->setOnSpfeIncCb(boost::bind(this->onSpfeInc, this));
        liServiceDiscovery<li_conn_info_t>::instance()->setOnSpfeIncCb(boost::bind(&connManager<connClass, serviceDiscovery>::onSpfeInc, this));
        liServiceDiscovery<li_conn_info_t>::instance()->setOnSpfeDecCb(boost::bind(&connManager<connClass, serviceDiscovery>::onSpfeDec, this));

        liServiceDiscovery<li_conn_info_t>::instance()->setOnXContainerIncCb(boost::bind(&connManager<connClass, serviceDiscovery>::onXInc, this));
        liServiceDiscovery<li_conn_info_t>::instance()->setOnXContainerDecCb(boost::bind(&connManager<connClass, serviceDiscovery>::onXDec, this));

        liServiceDiscovery<li_conn_info_t>::instance()->setOnProxyIncCb(boost::bind(&connManager<connClass, serviceDiscovery>::onProxyInc, this));
        liServiceDiscovery<li_conn_info_t>::instance()->setOnProxyDecCb(boost::bind(&connManager<connClass, serviceDiscovery>::onProxyDec, this));
    }

    void test()
    {
        __LOG(debug, "");
    }

    // protected:
    void onXInc(connList list)
    {
    }
    void onXDec(connList list)
    {
    }
    void onProxyInc(connList list)
    {
    }
    void onProxyDec(connList list)
    {
    }

    void onSpfeInc(connList list)
    {
        for (auto info : list)
        {
            li_conn_ind_t tmp_ind;
            if (connect_spfe(info, tmp_ind) == LI_SUCCESS)
            {
                // add the info to the map

                // add to the ind spfe list
            }
            else
            {
                // connect fail
            }
        }
    }

    void onSpfeDec(connList list)
    {
        for (auto info : list)
        {
            // close the connection
            // here needs to get out the token
            // to do
            // find the connection ind using info
            // loop the ind map
            if (true)//close(info) == LI_SUCCESS)
            {
            }
            else
            {
                //close fail, just return  delete it in the map
            }
        }
    }
    LI_RETURN connect_peer(li_conn_ind_t ind, TOKEN _token, conn_type type, li_conn_info_t connInfo)
    {

        __LOG(debug, "");
        li_conn_info_t connInfo_local;
        // how to get the local IP port info.
        // to do

        connClass::instance()->connect(&ind, _token, connInfo, connInfo_local);
        // insert it into the ind map or insert it when the connected callback is called?
        // now we choose insert it now and update when the connection callback is called(update work to do later).
        // suppose the ind will not change
        connAllInfo tmp_all_info;
        tmp_all_info.type = type;
        tmp_all_info.connInfo = connInfo;
        tmp_all_info._token = _token;
#ifdef TRANS
        OSENDPOINT _osep;
        if (info2OSEP(connInfo, &_osep))
        {
            tmp_all_info.osep = _osep;
        }
        else
        {
            //error log
            return LI_FAIL;
        }
#endif
        ind2ConnInfo[ind] = tmp_all_info;
        return LI_SUCCESS;
    }

    std::map<li_conn_ind_t, connAllInfo> ind2ConnInfo;
    // this is for select a connection and send to only one connection
    std::list<li_conn_ind_t> spfe_ind;
    std::list<li_conn_ind_t> proxy_list;
    // this is for send to all the connection
    std::list<li_conn_ind_t> x_list;
};
#endif
#if 0 
template <typename connectionMiddleWare, typename serviceDiscovery>
class connManager
{
  public:
    typedef typename connectionMiddleWare::conn_ind_t li_conn_ind_t;
    typedef typename connectionMiddleWare::conn_info_t li_conn_info_t;
    typedef typename serviceDiscovery::connList connList;

    bool init()
    {
        // 1. set connection inc/dec callback for service discovery calss
        servDiscov.setOnSpfeIncCb(boost::bind(&connManager<connectionMiddleWare, serviceDiscovery>::onSpfeInc, this));
        servDiscov.setOnSpfeDecCb(boost::bind(&connManager<connectionMiddleWare, serviceDiscovery>::onSpfeDec, this));
        // 2. App logic for example
        for (auto conn_info : (servDiscov.getSpfeInfo()))
        {
            li_conn_ind_t ind;
            (servDiscov.instance())->connect(&ind, conn_info);
            spfe_conn_ind_list.push(ind);
        }
        for (auto conn_info : (servDiscov.getXContainerInfo()))
        {
            li_conn_ind_t ind;
            (servDiscov.instance())->connect(&ind, conn_info);
            x_conn_ind_list.push(ind);
        }
    }
    void test()
    {
        __LOG(debug, "");
    }
    

    void onSpfeInc(connList list)
    {
    }
    void onSpfeDec(connList list)
    {
    }

    serviceDiscovery servDiscov;
    connectionMiddleWare liConn;

    std::list<li_conn_ind_t> spfe_conn_ind_list;
    std::list<li_conn_ind_t> scfe_conn_ind_list;
    std::list<li_conn_ind_t> tafe_conn_ind_list;
    std::list<li_conn_ind_t> x_conn_ind_list;
    std::list<li_conn_ind_t> proxy_conn_ind_list;
};
#endif
