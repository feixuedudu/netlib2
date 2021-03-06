#ifndef M_BUFFER_INCLUDE
#define M_BUFFER_INCLUDE

#include "config.hpp"
M_SOCKET_NAMESPACE_BEGIN
#define M_BUFFER_DEFAILT_SIZE 1024

class Buffer
{
public:
	M_SOCKET_DECL Buffer();

	M_SOCKET_DECL ~Buffer();

	M_SOCKET_DECL s_byte_t* Data();

	M_SOCKET_DECL const s_byte_t* Data()const;

	M_SOCKET_DECL s_uint32_t Capacity()const;

	M_SOCKET_DECL s_uint32_t Size()const;

	M_SOCKET_DECL s_uint32_t Length()const;

	M_SOCKET_DECL void Write(void* data, s_uint32_t len);

	template<typename T>
	M_SOCKET_DECL void Write(T const& value);

	template<typename T>
	Buffer& operator<<(T const& value)
	{
		Write(value);
		return *this;
	}

	M_SOCKET_DECL void Read(void* data, s_uint32_t len);

	template<typename T>
	M_SOCKET_DECL void Read(T& value);

	template<typename T>
	Buffer& operator >> (T& value)
	{
		Read(value);
		return *this;
	}

private:
	s_byte_t*	_data;
	s_uint32_t	_size;
	s_uint32_t	_pos;
	s_uint32_t	_offset;
};

M_SOCKET_DECL Buffer::Buffer()
	:_data(0),_size(0),_pos(0),_offset(0)
{
	_size = M_BUFFER_DEFAILT_SIZE;
	_data = (s_byte_t*)g_malloc(_size);
}

M_SOCKET_DECL Buffer::~Buffer()
{
	g_free(_data);
}

M_SOCKET_DECL s_byte_t* Buffer::Data()
{
	return _data;
}

M_SOCKET_DECL const s_byte_t* Buffer::Data()const
{
	return _data;
}

M_SOCKET_DECL s_uint32_t Buffer::Capacity()const
{
	return _size;
}

M_SOCKET_DECL s_uint32_t Buffer::Size()const
{
	return _pos;
}

M_SOCKET_DECL s_uint32_t Buffer::Length()const
{
	return _pos;
}

M_SOCKET_DECL void Buffer::Write(void* data, s_uint32_t len)
{
	if (_pos + len > _size)
	{
		_size = M_BUFFER_DEFAILT_SIZE*((_pos + len) / M_BUFFER_DEFAILT_SIZE + 1);
		s_byte_t* ptmp = (s_byte_t*)g_malloc(_size);
		g_memcpy(ptmp, _data, _pos);
		g_free(_data);
		_data = ptmp;
	}
	g_memcpy(_data + _pos, data, len);
	_pos += len;
}

template<typename T>
M_SOCKET_DECL void Buffer::Write(T const& value)
{
	Write(&value, sizeof(T));
}

M_SOCKET_DECL void Buffer::Read(void* data, s_uint32_t len)
{
	if (_offset + len > _pos)
		return;

	g_memcpy(data, _data + _offset, len);
	_offset += len;
}

template<typename T>
M_SOCKET_DECL void Buffer::Read(T& value)
{
	Read(&value, sizeof(T));
}


M_SOCKET_NAMESPACE_END
#endif