#ifndef TAMANDUA_BOX_HPP
#define TAMANDUA_BOX_HPP
#include "tamandua/tamandua.hpp"
#include "debug_gui.hpp"
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

struct tamandua_box
{
	boost::asio::io_service io_service;
	std::thread reader_thread, io_service_thread;
	std::mutex running_lock;
	boost::asio::ssl::context context;
	tamandua::client client;
	bool running;

	tamandua_box() :
		context(boost::asio::ssl::context::sslv23),
		client(io_service, context),
		running(true)
	{
		context.set_verify_mode(boost::asio::ssl::verify_peer);
		context.load_verify_file("ssl/tamandua_approved.crt");
	}

	void turn_off()
	{
		std::lock_guard<std::mutex> lock(running_lock);
		running = false;
	}
};

#endif
