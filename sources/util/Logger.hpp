/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <deque>

std::deque<std::string> *get_history();
#ifdef NDEBUG
	#define LOG_OPEN(a);
	#define LOG_CLOSE();
	#define LOG_FATAL(a);
	#define LOG_ERROR(a);
	#define LOG_STRING(a);
	#define LOG_WARNING(a);
#else
#ifdef LOG_LOCATION
	#define LOG_OPEN(a) log_open((a), __FILE__, __LINE__);
	#define LOG_CLOSE() log_close(__FILE__, __LINE__);
	#define LOG_FATAL(a) log_fatal((a), __FILE__, __LINE__);
	#define LOG_ERROR(a) log_error((a), __FILE__, __LINE__);
	#define LOG_STRING(a) log_string((a), __FILE__, __LINE__);
	#define LOG_WARNING(a) log_warning((a), __FILE__, __LINE__);
	void log_open(const std::string filename, const std::string f, int l);
	void log_close(const std::string f, int l);
	void log_fatal(const std::string message, const std::string f, int l);
	void log_error(const std::string message, const std::string f, int l);
	void log_string(const std::string message, const std::string f, int l);
	void log_warning(const std::string message, const std::string f, int l);
#else
	#define LOG_OPEN(a) log_open((a));
	#define LOG_CLOSE() log_close();
	#define LOG_FATAL(a) log_fatal((a));
	#define LOG_ERROR(a) log_error((a));
	#define LOG_STRING(a) log_string((a));
	#define LOG_WARNING(a) log_warning((a));
	void log_open(std::string filename);
	void log_close();
	void log_fatal(std::string message);
	void log_error(std::string message);
	void log_string(std::string message);
	void log_warning(std::string message);
#endif
#endif

#endif
