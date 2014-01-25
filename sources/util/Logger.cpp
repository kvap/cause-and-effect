/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <time.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "Logger.hpp"
#include "timing.hpp"
#include "convert.hpp"

static std::ofstream log;
static float start_s;

#define HISTORY_SIZE 1000
std::deque<std::string> history;

void save_message(std::string s) {
	history.push_front(s);
	if (history.size() > HISTORY_SIZE) {
		history.resize(HISTORY_SIZE);
	}
}

std::deque<std::string> *get_history() {
	return &history;
}

inline std::string ftos(float f) {
	std::ostringstream buffer;
	buffer << std::fixed << std::setprecision(3) << f;
	return buffer.str();
}

std::string GetTimeStr() {
	time_t curtime;
	curtime = time(NULL);
	return asctime(localtime(&curtime));
}

#ifdef LOG_LOCATION
void log_open(const std::string filename, const std::string f, int l) {
#else
void log_open(const std::string filename) {
#endif
	log.open(filename.c_str());

	if (!log) {
#ifdef LOG_LOCATION
		std::cerr << f << ", line " << l << " : ";
#endif
		std::cerr << "Error opening " << filename << " for logging" << std::endl;
		return;
	}

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << "---- " << GetTimeStr() << " : LOGGING STARTED." << std::endl; 
	log << buf.str();
	std::cout << buf.str();
	save_message(buf.str());

	start_s = now_s();
}

#ifdef LOG_LOCATION
void log_close(const std::string f, int l) {
#else
void log_close() {
#endif

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << "---- " << GetTimeStr() << " : LOGGING ENDED." << std::endl;
	log << buf.str();
	std::cout << buf.str();
	save_message(buf.str());

	log.close();
}

#ifdef LOG_LOCATION
void log_error(const std::string message, const std::string f, int l) {
#else
void log_error(const std::string message) {
#endif

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << ftos(now_s() - start_s) << " [ERROR] " << message << std::endl;
	log << buf.str();
	std::cerr << buf.str();
	save_message(buf.str());
}

#ifdef LOG_LOCATION
void log_fatal(const std::string message, const std::string f, int l) {
#else
void log_fatal(const std::string message) {
#endif

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << ftos(now_s() - start_s) << " [FATAL] " << message << std::endl;
	log << buf.str();
	std::cerr << buf.str();
	save_message(buf.str());

	LOG_CLOSE();
	exit(1);
}

#ifdef LOG_LOCATION
void log_string(const std::string message, const std::string f, int l) {
#else
void log_string(const std::string message) {
#endif

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << ftos(now_s() - start_s) << " : " << message << std::endl;
	log << buf.str();
	std::cout << buf.str();
	save_message(buf.str());
}

#ifdef LOG_LOCATION
void log_warning(const std::string message, const std::string f, int l) {
#else
void log_warning(const std::string message) {
#endif

	std::ostringstream buf;
#ifdef LOG_LOCATION
	buf << f << ", line " << l << " : ";
#endif
	buf << ftos(now_s() - start_s) << " [WARNING] " << message << std::endl;
	log << buf.str();
	std::cout << buf.str();
	save_message(buf.str());
}
