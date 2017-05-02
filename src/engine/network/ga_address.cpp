#include "ga_address.h"

ga_address::ga_address()
{
	_address = 0;
	_port = 0;
}

ga_address::ga_address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
{
	_address = (a << 24) | (b << 16) | (c << 8) | d;
	_port = port;
}

ga_address::ga_address(unsigned int address, unsigned short port)
{
	_address = address;
	_port = port;
}

unsigned int ga_address::get_address() const
{
	return _address;
}

unsigned char ga_address::get_a() const
{
	return (unsigned char) _address >> 24;
}
unsigned char ga_address::get_b() const
{
	return (unsigned char) _address >> 16;
}
unsigned char ga_address::get_c() const
{
	return (unsigned char) _address >> 8;
}
unsigned char ga_address::get_d() const
{
	return (unsigned char) _address;
}
unsigned short ga_address::get_port() const
{
	return _port;
}
