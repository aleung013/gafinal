#pragma once
// Address header from http://gafferongames.com/networking-for-game-programmers/sending-and-receiving-packets/
class ga_address
{
public:
	ga_address();
	ga_address(unsigned char a,
		unsigned char b,
		unsigned char c,
		unsigned char d,
		unsigned short port);
	ga_address(unsigned int address,
		unsigned short port);
	unsigned int get_address() const;
	unsigned char get_a() const;
	unsigned char get_b() const;
	unsigned char get_c() const;
	unsigned char get_d() const;
	unsigned short get_port() const;
private:
	unsigned int _address;
	unsigned short _port;
};