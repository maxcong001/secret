#pragma once
#include "li/util.hpp"

template <typename connInd, typename connInfo>
class connInterface
{
public:
  connInterface(){};
  virtual ~connInterface(){};
  /*-------------------------------------------------------------------------------------*/
  // common
  virtual LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual LI_RETURN close(connInd ind, TOKEN _token) = 0;

  /*-------------------------------------------------------------------------------------*/
  // as server
  virtual LI_RETURN listen(connInd *ind_ptr, TOKEN _token, connInfo info_local) = 0;

  /*-------------------------------------------------------------------------------------*/
  // as client
  virtual LI_RETURN connect(connInd *ind, TOKEN _token, connInfo info_remote, connInfo connInfo_local) = 0;

  // callback def
  virtual int onReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual int onConnectedCb(connInd ind, TOKEN _token) = 0;
  virtual int onConnClosedCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onConnErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onSocketErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onNewConnCb(connInd ind, TOKEN old_token, TOKEN *new_token, connInfo *info) = 0;
  virtual int onListenErrorCb(connInd ind, TOKEN _token, int errorNuber) = 0;
};
#if 0
// deleted versin two
template <typename connInd, typename connInfo>
class connInterface_client
{
public:
  connInterface_client(){};
  virtual ~connInterface_client(){};
  /*-------------------------------------------------------------------------------------*/

  virtual LI_RETURN send(connInd ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual LI_RETURN close(connInd ind) = 0;
  virtual LI_RETURN connect(connInd &ind, connInfo info) = 0;

  // client callback def
  virtual int onClientReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual int onClinetConnectedCb(connInd ind, TOKEN _token) = 0;
  virtual int OnClientConnClosedCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onClientConnErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onClientSocketErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;

  connInd client_ind;

};

template <typename connInd, typename connInfo>
class connInterface_server
{
public:
  connInterface_server(){};
  virtual ~connInterface_server(){};
  /*-------------------------------------------------------------------------------------*/

  virtual LI_RETURN send(connInd server_ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual LI_RETURN close(connInd ind) = 0;
  virtual LI_RETURN listen(connInd &ind, connInfo info) = 0;

  // server callback def
  virtual int onServerNewConnCb(connInd session_ind, TOKEN old_token, TOKEN *new_token, connInfo *info) = 0;
  virtual int OnServerReceiveCb(connInd session_ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual int OnServerConnClosedCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;
  virtual int onServerListenErrorCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;
  virtual int OnServerSocketErrorCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;

  connInd server_ind;


};
#endif
#if 0
/*delete version 1*/
template <typename connInd, typename connInfo>
class connInterface
{
public:

  connInterface(){};
  virtual ~connInterface(){};
  /*-------------------------------------------------------------------------------------*/
  // common
  virtual LI_RETURN send(connInd server_ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual LI_RETURN close(connInd ind) = 0;

  /*-------------------------------------------------------------------------------------*/
  // as server
  virtual LI_RETURN listen(connInd &ind, connInfo info) = 0;

  // server callback def
  virtual int onServerNewConnCb(connInd session_ind, TOKEN old_token, TOKEN *new_token, connInfo *info) = 0;
  virtual int OnServerReceiveCb(connInd session_ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual int OnServerConnClosedCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;
  virtual int onServerListenErrorCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;
  virtual int OnServerSocketErrorCb(connInd session_ind, TOKEN _token, int errorNuber) = 0;

  /*-------------------------------------------------------------------------------------*/
  // as client
  virtual LI_RETURN connect(connInd &ind, connInfo info) = 0;

  // client callback def
  virtual int onClientReceiveCb(connInd ind, TOKEN _token, void *msg, length_t len) = 0;
  virtual int onClinetConnectedCb(connInd ind, TOKEN _token) = 0;
  virtual int OnClientConnClosedCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onClientConnErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;
  virtual int onClientSocketErrorCb(connInd ind, TOKEN _token, int errorNumber) = 0;

  /*-------------------------------------------------------------------------------------*/
  // data
  // used when close server
  //connInd server_indicator;
  // server connection list
  //???
  // client connection list
  //???
};
#endif