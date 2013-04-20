#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

std::vector <std::string> txt2lines(const char* filename, const std::string& ignore, bool quiet = true)
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
				fprintf(stderr, "%04d %s\n", linecount, buffer);
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

bool load_scn(const char* filename)
{
	fprintf(stderr, "preparsing %s\n", filename);
	std::vector <std::string> lines = txt2lines(filename, "#", false);
	if (!lines.size())
	{
		fprintf(stderr, "preparse failed on %s\n", filename);
		return false;
	}
	fprintf(stderr, "preparse finished\n");
	fprintf(stderr, "parsing %s (%lu lines)\n", filename, lines.size());
	
	std::vector <std::string> ::iterator iter;
	for (iter = lines.begin(); iter != lines.end(); iter++)
	{
		
		std::string& line = *iter;
		fprintf(stderr, "> %s\n", line.c_str());
		std::vector <std::string> tokens = string2tokens(line, " \t");
		unsigned int tokencount = tokens.size();
		
		// is this a begin or end statement ?
		if (0x2 == tokencount)
		{
			std::string firsttoken = tokens.at(0);
			std::string secondtoken = tokens.at(1);
			std::transform(firsttoken.begin(), firsttoken.end(), firsttoken.begin(), tolower);
			std::transform(secondtoken.begin(), secondtoken.end(), secondtoken.begin(), tolower);
			
			if ("begin" == firsttoken)
			{
				if ("scene" == secondtoken)
				{
					
				}
				else if ("pickup" == secondtoken)
				{
				}
				else if ("exit" == secondtoken)
				{
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else if ("end" == firsttoken)
			{
				if ("scene" == secondtoken)
				{
					
				}
				else if ("pickup" == secondtoken)
				{
				}
				else if ("exit" == secondtoken)
				{
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
		}
		// this is a single value assignment Property = Value
		else if (0x3 == tokencount)
		{
			std::string property = tokens.at(0);
			std::string op = tokens.at(1);
			std::string value = tokens.at(2);
			std::transform(property.begin(), property.end(), property.begin(), tolower);
			
			if ("=" != op)
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
			
			if ("name" == property)
			{
				fprintf(stderr, "TODO: set scene name to \"%s\"\n", value.c_str());
			}
			else if ("background" == property)
			{
				fprintf(stderr, "TODO: set scene background to \"%s\"\n", value.c_str());
			}
			else if ("pickups" == property)
			{
				fprintf(stderr, "TODO: set scene pickup count to %d\n", atoi(value.c_str()));
			}
			else if ("exits" == property)
			{
				fprintf(stderr, "TODO: set scene exit count to %d\n", atoi(value.c_str()));
			}
			else if ("image" == property)
			{
				fprintf(stderr, "TODO: set scene background to \"%s\"\n", value.c_str());
			}
			else if ("x" == property)
			{
				fprintf(stderr, "TODO: set x to %d\n", atoi(value.c_str()));
			}
			else if ("y" == property)
			{
				fprintf(stderr, "TODO: set y to %d\n", atoi(value.c_str()));
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
			
		}
		// this is a multi value assignment Property = Value1 Value2 Value3 ValueN ...
		else if (tokencount > 3)
		{
			std::string property = tokens.at(0);
			std::string op = tokens.at(1);
			std::string value = tokens.at(2);
			std::transform(property.begin(), property.end(), property.begin(), tolower);
			
			if ("=" != op)
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
			
			for (unsigned int index = 3; index < tokencount; index++)
			{
				value = value + " " + tokens.at(index);
			}
			
			if ("bounds" == property)
			{
				if (5 > tokencount)
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
				fprintf(stderr, 
					"TODO: set pickup image collision bounds to [%d,%d,%d,%d]\n", 
					atoi(tokens.at(2).c_str()),
					atoi(tokens.at(3).c_str()),
					atoi(tokens.at(4).c_str()),
					atoi(tokens.at(5).c_str())
				);
			}
			else if ("dest" == property)
			{
				fprintf(stderr, "TODO: set exit destination to \"%s\"\n", value.c_str());
			}
			else if ("name" == property)
			{
				fprintf(stderr, "TODO: set scene name to \"%s\"\n", value.c_str());
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
			return false;
		}
	}
	
	fprintf(stderr, "parsing finished\n");
	
	return true;
}

int main(int argc, char* argv[])
{
	if (argc!=2){fprintf(stderr,"required filename parameter missing\n");return 1;}
	return (load_scn(argv[1])) ? 0 : 1;
}
