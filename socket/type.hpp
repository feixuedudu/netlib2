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

#ifndef M_TYPE_INCLUDE
#define M_TYPE_INCLUDE
M_SOCKET_NAMESPACE_BEGIN

#define shard_ptr_t std::tr1::shared_ptr 
#define enable_shared_from_this_t std::tr1::enable_shared_from_this
#define function_t	std::tr1::function
#define bind_t std::tr1::bind
#define placeholder_1 tr1::placeholders::_1
#define placeholder_2 tr1::placeholders::_2
#define placeholder_3 tr1::placeholders::_3
#define placeholder_4 tr1::placeholders::_4
#define placeholder_5 tr1::placeholders::_5
#define placeholder_6 tr1::placeholders::_6
#define placeholder_7 tr1::placeholders::_7
#define placeholder_8 tr1::placeholders::_8
#define placeholder_9 tr1::placeholders::_9
#define placeholder_10 tr1::placeholders::_10
#define dynamic_pointer_cast_t std::tr1::dynamic_pointer_cast

/*
基本数据类型
windows: 使用ILP32数据模型 及 LLP64数据模型
linux:	 平台使用 ILP32 及 LP64 数据模型
*/
#ifndef M_PLATFORM_DATA_MODE 
#define M_PLATFORM_DATA_MODE
typedef signed char				s_int8_t;
typedef signed short			s_int16_t;
typedef signed int				s_int32_t;
typedef signed long long		s_int64_t;
typedef unsigned char			s_uint8_t;
typedef unsigned short			s_uint16_t;
typedef unsigned int			s_uint32_t;
typedef unsigned long long		s_uint64_t;
typedef char					s_byte_t;
#endif  // M_PLATFORM_DATA_MODE

#ifdef M_PLATFORM_WIN
typedef OVERLAPPED	overlapped_t;
typedef WSAOVERLAPPED wsaoverlapped_t;
typedef WSABUF		wsabuf_t;
#endif

#ifndef M_PLATFORM_WIN
typedef epoll_event epoll_event_t;
typedef struct WSABUF { s_uint32_t len; s_byte_t* buf; } wsabuf_t;
#endif

// socket structure
typedef M_SOCKET_T				socket_t;
typedef sockaddr				sockaddr_t;
typedef sockaddr_in				sockaddr_in_t;
typedef sockaddr_in6			sockaddr_in6_t;
typedef sockaddr_storage		sockaddr_storage_t;
typedef struct linger			linger_t;

enum EShutdownType
{
	E_Shutdown_RD = M_SHUT_RD,
	E_Shutdown_WR = M_SHUT_WR,
	E_Ehutdown_BOTH = M_SHUT_RDWR
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_TCP_SOCKET_PTR(Service)    shard_ptr_t<TcpSocket<Service> >
#define M_TCP_ACCEPTOR_PTR(Service)  shard_ptr_t<TcpAcceptor<Service> >
#define M_TCP_CONNECTOR_PTR(Service) shard_ptr_t<TcpConnector<Service> >

#define M_ACCEPT_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_ACCEPTOR_PTR(Service), M_TCP_SOCKET_PTR(Service), SocketError)\
>
#define M_CONNECT_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_CONNECTOR_PTR(Service),SocketError)\
>
#define M_WRITE_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_SOCKET_PTR(Service),const s_byte_t*,s_uint32_t, s_uint32_t, SocketError)\
>
#define M_READ_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_SOCKET_PTR(Service),s_byte_t*,s_uint32_t, s_uint32_t, SocketError)\
>
#define M_C_WRITE_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_CONNECTOR_PTR(Service),const s_byte_t*,s_uint32_t, s_uint32_t, SocketError)\
>
#define M_C_READ_HANDLER_TYPE(Service) function_t<\
		void(M_TCP_CONNECTOR_PTR(Service),s_byte_t*,s_uint32_t, s_uint32_t, SocketError)\
>

#define M_CHECK_ACCEPT_HANDLER(handler,service) {\
	M_ACCEPT_HANDLER_TYPE(service) _h_##LINE_ = handler; \
}
#define M_CHECK_CONNECT_HANDLER(handler,service) {\
	M_CONNECT_HANDLER_TYPE(service) _h_##LINE_ = handler;\
}
#define M_CHECK_READ_HANDLER(handler,service) {\
	M_READ_HANDLER_TYPE(service) _h_##LINE_ = handler;\
}
#define M_CHECK_WRITE_HANDLER(handler,service) {\
	M_WRITE_HANDLER_TYPE(service) _h_##LINE_ = handler;\
}
#define M_CHECK_C_READ_HANDLER(handler,service) {\
	M_C_READ_HANDLER_TYPE(service) _h_##LINE_ = handler;\
} 
#define M_CHECK_C_WRITE_HANDLER(handler,service) {\
	M_C_WRITE_HANDLER_TYPE(service) _h_##LINE_ = handler;\
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_TCP_SOCKET(Service)    TcpSocket<Service>
#define M_TCP_ACCEPTOR(Service)  TcpAcceptor<Service>
#define M_TCP_CONNECTOR(Service) TcpConnector<Service>

#define M_COMMON_HANDLER_TYPE(Service) function_t<\
		void(SocketError)\
>
#define M_RW_HANDLER_TYPE(Service) function_t<\
		void(s_uint32_t,SocketError)\
>

#define M_CHECK_COMMON_HANDLER(handler,service) {\
	M_COMMON_HANDLER_TYPE(service) _h_##LINE_ = handler; \
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct HandlerTraits
{
	HandlerTraits()
	{
	}
	typedef T Type;
};

template<typename ReturnType, typename Arg1>
struct HandlerTraits<ReturnType(*)(Arg1)>
	//:public HandlerTraits<ReturnType(*)(Arg1)>
{
	typedef ReturnType(*type)(Arg1);
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
};

template<typename ReturnType, typename Arg1>
struct HandlerTraits<function_t<ReturnType(Arg1)> >
	//:public HandlerTraits<ReturnType(*)(Arg1)>
{
	typedef function_t<ReturnType(Arg1)> type;
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
};

template<typename ReturnType, typename Arg1, typename Arg2>
struct HandlerTraits<ReturnType(*)(Arg1, Arg2)>
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2)>
{
	typedef ReturnType(*type)(Arg1, Arg2);
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
};

template<typename ReturnType, typename Arg1, typename Arg2>
struct HandlerTraits<function_t<ReturnType(Arg1, Arg2)> >
	:public HandlerTraits<ReturnType(*)(Arg1, Arg2)>
{
	typedef function_t<ReturnType(Arg1, Arg2)> type;
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
};

template<typename ReturnType, typename Arg1, typename Arg2,typename Arg3>
struct HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3)>
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3)>
{
	typedef ReturnType(*type)(Arg1, Arg2, Arg3);
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
};

template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3>
struct HandlerTraits<function_t<ReturnType(Arg1, Arg2, Arg3)> >
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3)>
{
	HandlerTraits()
	{
	}
	typedef function_t<ReturnType(Arg1, Arg2, Arg3)> type;
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
};

template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3,typename Arg4>
struct HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4)>
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4)>
{
	typedef ReturnType(*type)(Arg1, Arg2, Arg3, Arg4);
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
	typedef Arg4 arg4_type;
};

template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct HandlerTraits<function_t<ReturnType(Arg1, Arg2, Arg3, Arg4)> >
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4)>
{
	typedef function_t<ReturnType(Arg1, Arg2, Arg3, Arg4)> type;
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
	typedef Arg4 arg4_type;
};

template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4,typename Arg5>
struct HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
{
	typedef ReturnType(*type)(Arg1, Arg2, Arg3, Arg4, Arg5);
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
	typedef Arg4 arg4_type;
	typedef Arg5 arg5_type;
};

template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct HandlerTraits<function_t<ReturnType(Arg1, Arg2, Arg3, Arg4, Arg5)> >
	//:public HandlerTraits<ReturnType(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
{
	typedef function_t<ReturnType(Arg1, Arg2, Arg3, Arg4, Arg5)> type;
	typedef ReturnType return_type;
	typedef Arg1 arg1_type;
	typedef Arg2 arg2_type;
	typedef Arg3 arg3_type;
	typedef Arg4 arg4_type;
	typedef Arg5 arg5_type;
};

#define M_HANDLER_SOCKET_PTR(HANDLER)\
	typename HandlerTraits<HANDLER>::arg1_type 
#define M_HANDLER_SOCKET_PTR2(HANDLER)\
	typename HandlerTraits<HANDLER>::arg2_type

M_SOCKET_NAMESPACE_END
#endif