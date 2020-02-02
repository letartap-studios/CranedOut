#pragma once
#include "Rectangle.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

inline float getNumber(std::string &str)
{
	std::string temp;
	int i = 0;
	while (str[i] != ',' && str[i] != ' ' && str[i] != '\n')
	{
		temp += str[i];
		i++;
	}

	str.erase(str.begin() + temp.length() + 2);
	return stof(temp);
}


inline void levelCreate(std::vector<RectangleBody>& bodies,int &gameWidth, int &gameHeight, int &constructionStart, int &constructionEnd, std::string filename)
{
	std::ifstream file(filename);

	std::string line;

	while(!file.eof())
	{
		std::getline(file, line);
		std::stringstream s;
		s << line;
		std::cout << s.str() << "\n";
		std::string command;
		s >> command;

		if (command.find("gameWidth:") != std::string::npos)
		{
			s >> gameWidth;
			std::cout << command << "\n";
		}
		else if (command.find("gameHeight:") != std::string::npos)
		{
			s >> gameHeight;
		}
		else if (command.find("constructionStart:") != std::string::npos)
		{
			s >> constructionStart;
		}
		else if (command.find("constructionEnd:") != std::string::npos)
		{	
			s >> constructionEnd;
		}
		else if (command.find("addBlock:") != std::string::npos)
		{
			int x, y, w, h;
			float d;
			s >> x >> y >> w >> h >> d;
			bodies.push_back({ x,y,w,h,d });
		}
		line.clear();
	}


	/*
	char c;
	file >> c;
	
	while(!file.eof())
	{
		std::string str;
		while (c != '\n')
		{
			str += c;
			file >> c;
		}

		if(str.find("gameWidth: "))
		{
			str.erase(str.begin() + static_cast<std::string>("gameWidth: ").length());
			gameWidth = std::stoi(str);
		}
		else if (str.find("gameHeight: "))
		{
			str.erase(str.begin() + static_cast<std::string>("gameHeight: ").length());
			gameHeight = std::stoi(str);
		}
		else if (str.find("constructionStart: "))
		{
			str.erase(str.begin() + static_cast<std::string>("constructionStart: ").length());
			constructionStart = std::stoi(str);
		}
		else if (str.find("constructionEnd: "))
		{
			str.erase(str.begin() + static_cast<std::string>("constructionEnd: ").length());
			constructionEnd = std::stoi(str);
		}
		else if (str.find("addBlock: "))
		{
			str.erase(str.begin() + static_cast<std::string>("addBlock: ").length());

			const float xPos = getNumber(str);
			const float yPos = getNumber(str);
			const float width = getNumber(str);
			const float height = getNumber(str);
			const float density = getNumber(str);
			
			RectangleBody body(xPos, yPos, width, height, density);
			
			bodies.push_back(body);
		}
		

		file >> c;
	}
	*/

	file.close();

}
