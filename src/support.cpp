
#include "support.h"


void makebox(int* target, int left, int top, int right, int bottom)
{
	target[0] = left;
	target[1] = top;
	target[2] = right;
	target[3] = bottom;
}

////////////////////////////////////////////////////////////////////////////////

std::string truncate_string(const char* s)
{
	std::string o = s;
	std::string whitespacechars(" \t\f\v\n\r");
	size_t startpos = o.find_first_not_of(whitespacechars);
	size_t endpos = o.find_last_not_of(whitespacechars);
	if (std::string::npos != startpos && std::string::npos != endpos)
	{
		o.erase(endpos + 1);
		o = o.substr(startpos);
		return o;
	}
	return "";
}

////////////////////////////////////////////////////////////////////////////////

std::vector <std::string> txt2lines(const char* filename, const std::string& ignore, bool quiet)
{
	int linecount = 0;
	std::vector <std::string> filelines;
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		if (!quiet)
		{
			fprintf(stderr, "Error: Cannot Open %s for Reading!\n", filename);
		}
	}
	else
	{
		char buffer[0x1000];
		while(fgets(buffer, sizeof(buffer), fp))
		{
			char ichar = ignore.c_str()[0];
			if (ichar == buffer[0])
			{
				if (!quiet)
				{
					fprintf(stderr, "Ignoring: %s\n", buffer);
				}
			}
			else
			{
				//fprintf(stderr, "%04d %s\n", linecount, buffer);
				std::string line = truncate_string(buffer);
				if (line.size())
				{
					filelines.push_back(line);
				}
				linecount++;
			}
		}
		fclose(fp);
	}
	fprintf(stderr, "preparse stats: %d lines in %lu lines out\n", linecount, filelines.size());
	return filelines;
}


std::vector <std::string> string2tokens(const std::string& source, const std::string& delimiters)
{
	std::vector <std::string> tokens;
	size_t p0 = 0;
	size_t p1 = std::string::npos;
	while (p0 != std::string::npos)
	{
		p1 = source.find_first_of(delimiters, p0);
		if (p1 != p0)
		{
			std::string token = source.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = source.find_first_not_of(delimiters, p1);
	}
	return tokens;
}
