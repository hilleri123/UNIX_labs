
#pragma once


struct bd_file {
	int id;
	char name[256];
	std::size_t size;
};

const std::size_t count = 100;
const std::size_t bd_size = sizeof(bd_file)*count;

const std::string bd_name = "/data_base";

