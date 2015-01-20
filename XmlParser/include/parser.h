#pragma once

#include <istream>
#include <memory>
#include <exception>

#include <dom.h>

class no_token_exception : public std::exception
{};

class invalid_token_exception : public std::exception
{};

class Parser
{
public:
	std::shared_ptr<Element> parse(std::istream& input);
};