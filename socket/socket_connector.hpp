/*----------------------------------------------------------------
// Copyright (C) 2017 public
//
// 修改人：xiaoquanjie
// 时间：2017/8/10
//
// 修改人：xiaoquanjie
// 时间：
// 修改说明：
//
// 版本：V1.0.0
//----------------------------------------------------------------*/

#ifndef M_SOCKET_CONNECTOR_INCLUDE
#define M_SOCKET_CONNECTOR_INCLUDE

#include "socket_stream.hpp"
#include "protocol.hpp"
#include "connector_service.hpp"
M_SOCKET_NAMESPACE_BEGIN

template<typename IoServiceType>
class TcpConnector : public StreamSocket<Tcp, TcpConnectorService<Tcp, IoServiceType> >,
	public enable_shared_from_this_t<TcpConnector<IoServiceType> >
{
public:
	typedef TcpConnector<IoServiceType> MySelf;
	typedef StreamSocket<Tcp, TcpConnectorService<Tcp, IoServiceType> > SocketType;
	typedef Tcp::EndPoint EndPoint;

	M_SOCKET_DECL TcpConnector(IoServiceType& ioservice);

	M_SOCKET_DECL void Connect(const EndPoint& ep);

	M_SOCKET_DECL void Connect(const EndPoint& ep, SocketError& error);
		
	/*ConnectHandler: void(TcpConnectorPtr,SocketError)*/
	template<typename ConnectHandler>
	M_SOCKET_DECL void AsyncConnect(const EndPoint& ep, ConnectHandler handler);

	/*ConnectHandler: void(TcpConnectorPtr,SocketError)*/
	template<typename ConnectHandler>
	M_SOCKET_DECL void AsyncConnect(const EndPoint& ep, ConnectHandler handler,SocketError& error);

	/*ConnectHandler: void(SocketError)*/
	M_SOCKET_DECL void AsyncConnect(M_COMMON_HANDLER_TYPE(IoServiceType) handler, const EndPoint& ep);

	/*ConnectHandler: void(SocketError)*/
	M_SOCKET_DECL void AsyncConnect(M_COMMON_HANDLER_TYPE(IoServiceType) handler, const EndPoint& ep, SocketError& error);

	/*ReadHandler: void(TcpConnectorPtr connector, s_byte_t* data, s_uint32_t max, s_uint32_t trans, SocketError error)*/
	template<typename ReadHandler>
	M_SOCKET_DECL void AsyncRecvSome(s_byte_t* data, s_uint32_t size, ReadHandler handler);

	/*ReadHandler: void(TcpConnectorPtr connector, s_byte_t* data, s_uint32_t max, s_uint32_t trans, SocketError error)*/
	template<typename ReadHandler>
	M_SOCKET_DECL void AsyncRecvSome(s_byte_t* data, s_uint32_t size, ReadHandler handler, SocketError& error);

	/*ReadHandler: void(s_uint32 trans,SocketError error)*/
	M_SOCKET_DECL void AsyncRecvSome(M_RW_HANDLER_TYPE(IoServiceType) handler,s_byte_t* data, s_uint32_t size);

	/*ReadHandler: void(s_uint32 trans,SocketError error)*/
	M_SOCKET_DECL void AsyncRecvSome(M_RW_HANDLER_TYPE(IoServiceType) handler, s_byte_t* data, s_uint32_t size, SocketError& error);

	/*WriteHandler: void(TcpConnectorPtr connector, const s_byte_t* data, s_uint32_t max, s_uint32_t trans, SocketError error)*/
	template<typename WriteHandler>
	M_SOCKET_DECL void AsyncSendSome(const s_byte_t* data, s_uint32_t size, WriteHandler handler);

	/*WriteHandler: void(TcpConnectorPtr connector, const s_byte_t* data, s_uint32_t max, s_uint32_t trans, SocketError error)*/
	template<typename WriteHandler>
	M_SOCKET_DECL void AsyncSendSome(const s_byte_t* data, s_uint32_t size, WriteHandler handler, SocketError& error);

	/*WriteHandler: void(s_uint32 trans,SocketError error)*/
	M_SOCKET_DECL void AsyncSendSome(M_RW_HANDLER_TYPE(IoServiceType) handler, s_byte_t* data, s_uint32_t size);

	/*WriteHandler: void(s_uint32 trans,SocketError error)*/
	M_SOCKET_DECL void AsyncSendSome(M_RW_HANDLER_TYPE(IoServiceType) handler, const s_byte_t* data, s_uint32_t size, SocketError& error);
};

template<typename IoServiceType>
M_SOCKET_DECL TcpConnector<IoServiceType>::TcpConnector(IoServiceType& ioservice)
	:SocketType(ioservice)
{
	this->GetObjectService().Construct(this->GetImpl(), E_CONNECTOR_TYPE);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::Connect(const EndPoint& ep)
{
	SocketError error;
	this->Connect(ep, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::Connect(const EndPoint& ep, SocketError& error)
{
	if (!this->IsOpen())
	{
		Tcp pt = ep.Protocol();
		this->GetObjectService().Open(this->GetImpl(), pt, error);
		M_THROW_DEFAULT_SOCKET_ERROR2(error);
	}
	this->GetObjectService().Connect(this->GetImpl(), ep, error);
}

template<typename IoServiceType>
template<typename ConnectHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncConnect(const EndPoint& ep, ConnectHandler handler)
{
	M_CHECK_CONNECT_HANDLER(handler, IoServiceType);
	SocketError error;
	this->AsyncConnect(ep, handler,error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
template<typename ConnectHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncConnect(const EndPoint& ep, ConnectHandler handler,SocketError& error)
{
	M_CHECK_CONNECT_HANDLER(handler, IoServiceType);
	if (!this->IsOpen())
	{
		Tcp pt = ep.Protocol();
		this->GetObjectService().Open(this->GetImpl(), pt, error);
		M_THROW_DEFAULT_SOCKET_ERROR2(error);
	}
	this->GetObjectService().AsyncConnect(this->shared_from_this(), ep, handler, error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncConnect(M_COMMON_HANDLER_TYPE(IoServiceType) handler, const EndPoint& ep)
{
	SocketError error;
	this->AsyncConnect(handler, ep, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncConnect(M_COMMON_HANDLER_TYPE(IoServiceType) handler, const EndPoint& ep, SocketError& error)
{
	if (!this->IsOpen())
	{
		Tcp pt = ep.Protocol();
		this->GetObjectService().Open(this->GetImpl(), pt, error);
		M_THROW_DEFAULT_SOCKET_ERROR2(error);
	}
	this->GetObjectService().AsyncConnect(this->GetImpl(), handler, ep, error);
}

template<typename IoServiceType>
template<typename ReadHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncRecvSome(s_byte_t* data, s_uint32_t size, ReadHandler handler)
{
	M_CHECK_C_READ_HANDLER(handler, IoServiceType);
	SocketError error;
	this->AsyncRecvSome(data, size, handler, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
template<typename ReadHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncRecvSome(s_byte_t* data, s_uint32_t size, ReadHandler handler, SocketError& error)
{
	M_CHECK_C_READ_HANDLER(handler, IoServiceType);
	this->GetObjectService().AsyncRecvSome(this->shared_from_this(), data, size, handler, error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncRecvSome(M_RW_HANDLER_TYPE(IoServiceType) handler, s_byte_t* data, s_uint32_t size)
{
	SocketError error;
	this->AsyncRecvSome(handler, data, size, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncRecvSome(M_RW_HANDLER_TYPE(IoServiceType) handler, s_byte_t* data, s_uint32_t size, SocketError& error)
{
	this->GetObjectService().AsyncRecvSome(this->GetImpl(), data, size, handler, error);
}

template<typename IoServiceType>
template<typename WriteHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncSendSome(const s_byte_t* data, s_uint32_t size, WriteHandler handler)
{
	M_CHECK_C_WRITE_HANDLER(handler, IoServiceType);
	SocketError error;
	this->AsyncSendSome(data, size, handler, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
template<typename WriteHandler>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncSendSome(const s_byte_t* data, s_uint32_t size, WriteHandler handler, SocketError& error)
{
	M_CHECK_C_WRITE_HANDLER(handler, IoServiceType);
	this->GetObjectService().AsyncSendSome(this->shared_from_this(), data, size, handler, error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncSendSome(M_RW_HANDLER_TYPE(IoServiceType) handler, s_byte_t* data, s_uint32_t size)
{
	SocketError error;
	this->AsyncSendSome(handler, data, size, error);
	M_THROW_DEFAULT_SOCKET_ERROR2(error);
}

template<typename IoServiceType>
M_SOCKET_DECL void TcpConnector<IoServiceType>::AsyncSendSome(M_RW_HANDLER_TYPE(IoServiceType) handler, const s_byte_t* data, s_uint32_t size, SocketError& error)
{
	this->GetObjectService().AsyncSendSome(this->GetImpl(), data, size, handler, error);
}

M_SOCKET_NAMESPACE_END
#endif