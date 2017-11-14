#pragma once
#include <list>
#include <map>
#include <boost/function.hpp>
#include <li/util.hpp>
template <typename connInfo>
class ICSrvcInfo
{
public:
  typedef typename connInfo::srvcType_t serviceType;
  typedef std::list<connInfo> connList;
  typedef boost::function<void(serviceType, connList)> connInfoChgCb;
  typedef std::map<serviceType, connList> connMap;
  typedef connInfo conn_info_t;

  static ICSrvcInfo<connInfo> *instance()
  {
    static ICSrvcInfo<connInfo> *ins = new ICSrvcInfo<connInfo>();
    return ins;
  }

  bool setSrvcInfo(serviceType type, connList loIt)
  {
#if 0
    LI_LOCK(ics_mutex);
    connMapIt mapIt = srvcInfoMap.find(type);
    if (mapIt != srvcInfoMap.end())
    {
      ICSrvcInfo<connInfo>::connList &loList = mapIt->second;
      ICSrvcInfo<connInfo>::connListIt loIt;
      for (ICSrvcInfo<connInfo>::connListIt inIt = inList.begin(); inIt != inList.end(); inIt++)
      {
        for (loIt = loList.begin(); loIt != loList.end(); loIt++)
        {
          if (*inIt == *loIt)
          { //update the value, but not invoke cb
            loList.erase(loIt);
            loList.push_back(*inIt);
            break;
          }
        }
        if (loIt == loList.end())
        {
          loList.push_back(*inIt);
          newList.push_back(*inIt);
        }
      }
      if (srvc_conn_inc_cb != NULL && !newList.empty())
      {
        srvc_conn_inc_cb(type, newList);
      }
    }
    else
    {
      srvcInfoMap.insert(std::pair<SRVC_TYPE::type, connList>(type, inList));
      if (srvc_conn_inc_cb != NULL)
        srvc_conn_inc_cb(type, inList);
    }
#endif
    return true;
  }
  bool delSrvcInfo(serviceType, connList){};

  connList getSrvcInfo(serviceType)
  {
    connInfo info;
    info.ip = "127.0.0.1";
    connList temp_list;
    temp_list.push_back(info);
    return temp_list;
  }

  bool setOnSrvcIncCb(connInfoChgCb cb) { srvc_conn_inc_cb = cb; }
  bool setOnSrvcDecCb(connInfoChgCb cb) { srvc_conn_dec_cb = cb; }

  connInfoChgCb srvc_conn_inc_cb;
  connInfoChgCb srvc_conn_dec_cb;
  connMap srvcInfoMap;
  boost::mutex ics_mutex;
};

#if 0
// deleted version one
template <typename connInfo>
class liServiceDiscovery
{
public:
  typedef std::list<connInfo> connList;
  typedef boost::function<void(connList)> connInfoChgCb;
  static liServiceDiscovery<connInfo> *instance()
  {
    static liServiceDiscovery<connInfo> *ins = new liServiceDiscovery<connInfo>();
    return ins;
  }

  bool setSpfeInfo(connList);
  bool setXContainerInfo(connList);
  bool setProxyInfo(connList);

  connList getSpfeInfo();
  connList getXContainerInfo();
  connList getProxyInfo();
  connList getFesInfo();

  bool setOnSpfeIncCb(connInfoChgCb cb) { spfe_conn_inc_cb = cb; };
  bool setOnSpfeDecCb(connInfoChgCb cb) { spfe_conn_dec_cb = cb; };
  bool setOnXContainerIncCb(connInfoChgCb cb) { spfe_conn_inc_cb = cb; };
  bool setOnXContainerDecCb(connInfoChgCb cb) { spfe_conn_dec_cb = cb; };
  bool setOnProxyIncCb(connInfoChgCb cb) { spfe_conn_inc_cb = cb; };
  bool setOnproxyDecCb(connInfoChgCb cb) { spfe_conn_dec_cb = cb; };

  connList conn_info_x_container;
  connList conn_info_spfe;
  connList conn_info_proxy;
  connList conn_info_fes;

  connInfoChgCb spfe_conn_inc_cb;
  connInfoChgCb spfe_conn_dec_cb;
  connInfoChgCb x_container_conn_inc_cb;
  connInfoChgCb x_container_conn_dec_cb;
  connInfoChgCb proxy_conn_inc_cb;
  connInfoChgCb proxy_conn_dec_cb;
  /*
  // optional for further usage
  connList getScfeInfo(){};
  connList getTafeInfo(){};
  connList getProxyInfo(){};
  connList getFEsInfo(){};
  connList conn_info_scfe;
  connList conn_info_tafe;
  connList conn_info_proxy;
  // end optional
  */
};

#endif
