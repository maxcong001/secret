#pragma once
#include "li/util.hpp"
// call back functions
// there will be five sets of callback group
/***********************************************************************************************************************/
// this is: we are client and connect to proxy
int LI_Client_Proxy_onNewConnNotifyCB(void *newLlc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddr)
{
}
int LI_Client_Proxy_onConnectNotifyCB(void *llc, void *ulc)
{
}
int LI_Client_Proxy_onCloseNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_Proxy_onConnectErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_Proxy_onListenErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_Proxy_onTimeoutNotifyCB(void *llc, void *ulc, int timerId)
{
}
int LI_Client_Proxy_onReceived(void *llc, void *ulc, char *msg, int size)
{
}
int LI_Client_Proxy_onReceivedUDP(void *llc, void *ulc, OSENDPOINT *orig, OSENDPOINT *dest, char *msg, int size)
{
}
int LI_Client_Proxy_onSocketError(void *llc, void *ulc, int error)
{
}
/***********************************************************************************************************************/
// build trans event
LI_RETURN build_LI_Client_Proxy_TransEvent(TransEventT &LI_trans_event)
{
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyNewConnection = LI_Client_Proxy_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = LI_Client_Proxy_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = LI_Client_Proxy_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = LI_Client_Proxy_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = LI_Client_Proxy_onListenErrorNotifyCB;
    LI_trans_event.TransNotifyTimeout = LI_Client_Proxy_onTimeoutNotifyCB;
    LI_trans_event.TransNotifyPacketArrived = LI_Client_Proxy_onReceived;
    LI_trans_event.TransNotifyUDPPacketArrived = LI_Client_Proxy_onReceivedUDP;
    LI_trans_event.TransNotifySocketError = LI_Client_Proxy_onSocketError;
}
/***********************************************************************************************************************/
// this is: we are client and connect to IC container
int LI_Client_IC_onNewConnNotifyCB(void *newLlc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddr)
{
}
int LI_Client_IC_onConnectNotifyCB(void *llc, void *ulc)
{
}
int LI_Client_IC_onCloseNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_IC_onConnectErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_IC_onListenErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_IC_onTimeoutNotifyCB(void *llc, void *ulc, int timerId)
{
}
int LI_Client_IC_onReceived(void *llc, void *ulc, char *msg, int size)
{
}
int LI_Client_IC_onReceivedUDP(void *llc, void *ulc, OSENDPOINT *orig, OSENDPOINT *dest, char *msg, int size)
{
}
int LI_Client_IC_onSocketError(void *llc, void *ulc, int error)
{
}
/***********************************************************************************************************************/
// build trans event
LI_RETURN build_LI_Client_IC_TransEvent(TransEventT &LI_trans_event)
{
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyNewConnection = LI_Client_IC_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = LI_Client_IC_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = LI_Client_IC_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = LI_Client_IC_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = LI_Client_IC_onListenErrorNotifyCB;
    LI_trans_event.TransNotifyTimeout = LI_Client_IC_onTimeoutNotifyCB;
    LI_trans_event.TransNotifyPacketArrived = LI_Client_IC_onReceived;
    LI_trans_event.TransNotifyUDPPacketArrived = LI_Client_IC_onReceivedUDP;
    LI_trans_event.TransNotifySocketError = LI_Client_IC_onSocketError;
}
/***********************************************************************************************************************/
// this is: we are client and connect to SPFE
int LI_Client_SPFE_onNewConnNotifyCB(void *newLlc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddr)
{
}
int LI_Client_SPFE_onConnectNotifyCB(void *llc, void *ulc)
{
}
int LI_Client_SPFE_onCloseNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_SPFE_onConnectErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_SPFE_onListenErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Client_SPFE_onTimeoutNotifyCB(void *llc, void *ulc, int timerId)
{
}
int LI_Client_SPFE_onReceived(void *llc, void *ulc, char *msg, int size)
{
}
int LI_Client_SPFE_onReceivedUDP(void *llc, void *ulc, OSENDPOINT *orig, OSENDPOINT *dest, char *msg, int size)
{
}
int LI_Client_SPFE_onSocketError(void *llc, void *ulc, int error)
{
}
/***********************************************************************************************************************/
// build trans event
LI_RETURN build_LI_Client_SPFE_TransEvent(TransEventT &LI_trans_event)
{
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyNewConnection = LI_Client_SPFE_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = LI_Client_SPFE_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = LI_Client_SPFE_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = LI_Client_SPFE_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = LI_Client_SPFE_onListenErrorNotifyCB;
    LI_trans_event.TransNotifyTimeout = LI_Client_SPFE_onTimeoutNotifyCB;
    LI_trans_event.TransNotifyPacketArrived = LI_Client_SPFE_onReceived;
    LI_trans_event.TransNotifyUDPPacketArrived = LI_Client_SPFE_onReceivedUDP;
    LI_trans_event.TransNotifySocketError = LI_Client_SPFE_onSocketError;
}
/***********************************************************************************************************************/
// this is: we are server and connect to IC container
int LI_Server_IC_onNewConnNotifyCB(void *newLlc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddr)
{
}
int LI_Server_IC_onConnectNotifyCB(void *llc, void *ulc)
{
}
int LI_Server_IC_onCloseNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_IC_onConnectErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_IC_onListenErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_IC_onTimeoutNotifyCB(void *llc, void *ulc, int timerId)
{
}
int LI_Server_IC_onReceived(void *llc, void *ulc, char *msg, int size)
{
}
int LI_Server_IC_onReceivedUDP(void *llc, void *ulc, OSENDPOINT *orig, OSENDPOINT *dest, char *msg, int size)
{
}
int LI_Server_IC_onSocketError(void *llc, void *ulc, int error)
{
}
/***********************************************************************************************************************/
// build trans event
LI_RETURN build_LI_Server_IC_TransEvent(TransEventT &LI_trans_event)
{
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyNewConnection = LI_Server_IC_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = LI_Server_IC_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = LI_Server_IC_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = LI_Server_IC_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = LI_Server_IC_onListenErrorNotifyCB;
    LI_trans_event.TransNotifyTimeout = LI_Server_IC_onTimeoutNotifyCB;
    LI_trans_event.TransNotifyPacketArrived = LI_Server_IC_onReceived;
    LI_trans_event.TransNotifyUDPPacketArrived = LI_Server_IC_onReceivedUDP;
    LI_trans_event.TransNotifySocketError = LI_Server_IC_onSocketError;
}
/***********************************************************************************************************************/
// this is: we are server and connect to FEs
int LI_Server_FEs_onNewConnNotifyCB(void *newLlc, void *listeningUlc, void **returnedUlc, OSENDPOINT *remoteAddr)
{
}
int LI_Server_FEs_onConnectNotifyCB(void *llc, void *ulc)
{
}
int LI_Server_FEs_onCloseNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_FEs_onConnectErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_FEs_onListenErrorNotifyCB(void *llc, void *ulc, int errorNum)
{
}
int LI_Server_FEs_onTimeoutNotifyCB(void *llc, void *ulc, int timerId)
{
}
int LI_Server_FEs_onReceived(void *llc, void *ulc, char *msg, int size)
{
}
int LI_Server_FEs_onReceivedUDP(void *llc, void *ulc, OSENDPOINT *orig, OSENDPOINT *dest, char *msg, int size)
{
}
int LI_Server_FEs_onSocketError(void *llc, void *ulc, int error)
{
}
/***********************************************************************************************************************/
// build trans event
LI_RETURN build_LI_Server_FEs_TransEvent(TransEventT &LI_trans_event)
{
    // Set translib callback functions
    memset(&LI_trans_event, 0, sizeof(TransEventT));
    LI_trans_event.TransNotifyNewConnection = LI_Server_FEs_onNewConnNotifyCB;
    LI_trans_event.TransNotifyConnected = LI_Server_FEs_onConnectNotifyCB;
    LI_trans_event.TransNotifyConnectionClosed = LI_Server_FEs_onCloseNotifyCB;
    LI_trans_event.TransNotifyConnectError = LI_Server_FEs_onConnectErrorNotifyCB;
    LI_trans_event.TransNotifyListenError = LI_Server_FEs_onListenErrorNotifyCB;
    LI_trans_event.TransNotifyTimeout = LI_Server_FEs_onTimeoutNotifyCB;
    LI_trans_event.TransNotifyPacketArrived = LI_Server_FEs_onReceived;
    LI_trans_event.TransNotifyUDPPacketArrived = LI_Server_FEs_onReceivedUDP;
    LI_trans_event.TransNotifySocketError = LI_Server_FEs_onSocketError;
}
/***********************************************************************************************************************/
// build trans options
LI_RETURN buildTransOption(TransSockOpts &options, bool withTPKT, unsigned short type = 0)
{
    memset(&options, 0, sizeof(TransSockOpts));
    if (withTPKT)
    {
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
    else
    {
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
    return LI_SUCCESS;
}