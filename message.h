//
// Created by keroshi on 2026/5/11.
//

#ifndef KEROSHIZEROTETRIS_MESSAGE_H
#define KEROSHIZEROTETRIS_MESSAGE_H

#include <iostream>
#include <chrono>
#include <iomanip>

struct Message {
	enum Level {
		INFO, WARNING, ERROR
	};

	template < typename ... Args>
	static void log(Level level, bool show_time, Args ... args) {
		std :: ostream & os = level == INFO ? std :: cout : std :: cerr;

		if (show_time) {
			auto now = std :: chrono :: system_clock :: now();
			auto time_t = std :: chrono :: system_clock :: to_time_t(now);
			os << "["
				<< std :: put_time(std :: localtime(& time_t), "%H:%M:%S")
				<< "] ";
		}

		if (level == INFO) os << "[INFO] ";
		if (level == WARNING) os << "[WARN] ";
		if (level == ERROR) os << "[ERROR] ";

		(os << ... << args) << std :: endl;
	}
};

#endif //KEROSHIZEROTETRIS_MESSAGE_H
