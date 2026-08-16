//
// Created by keroshi on 2026/8/9.
//

#ifndef KEROSHIZEROTETRIS_LOGGER_HPP
#define KEROSHIZEROTETRIS_LOGGER_HPP

#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>

namespace Logger {
	enum class Level {
		INFO,
		WARNING,
		ERROR,
		DEBUG,
		PROGRESS,
	};

	template < typename ... Args>
	void log(Level level, Args && ... args) {
		std :: ostream & os = level == Level :: INFO ? std :: cout : std :: cerr;

		if (level == Level :: PROGRESS) {
			os << "\r\033[K";
		}

		auto now = std :: chrono :: system_clock :: now();
		auto time_t = std :: chrono :: system_clock :: to_time_t(now);

		os << "[" << std :: put_time(std :: localtime(& time_t), "%H:%M:%S") << "] ";

		if (level == Level :: INFO) os << "[INFO] ";
		if (level == Level :: WARNING) os << "[WARN] ";
		if (level == Level :: ERROR) os << "[ERROR] ";
		if (level == Level :: DEBUG) os << "[DEBUG] ";
		if (level == Level :: PROGRESS) os << "[PROGRESS] ";

		(os << ... << args);

		if (level == Level :: PROGRESS) {
			os << std :: flush;
		}
		else {
			os << std :: endl;
		}
	}

	template < typename ... Args>
	void info(Args && ... args) {
		log(Level :: INFO, args ...);
	}

	template < typename ... Args>
	void warning(Args && ... args) {
		log(Level :: WARNING, args ...);
	}

	template < typename ... Args>
	void error(Args && ... args) {
		log(Level :: ERROR, args ...);
	}

	template < typename ... Args>
	void debug(Args && ... args) {
		log(Level :: DEBUG, args ...);
	}

	template < typename ... Args>
	void progress(Args && ... args) {
		log(Level :: PROGRESS, args ...);
	}

}

#endif //KEROSHIZEROTETRIS_LOGGER_HPP
