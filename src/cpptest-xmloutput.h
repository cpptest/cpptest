// ---
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2019 Esri
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---

#ifndef CPPTEST_XMLOUTPUT_H
#define CPPTEST_XMLOUTPUT_H

/**
This is extension for the test to allow XML output

added by Esri
*/

#include <iostream>
#include <list>

#include "cpptest-source.h"
#include "cpptest-output.h"
#include "cpptest-textoutput.h"
#include "cpptest-collectoroutput.h"
#include "cpptest-time.h"
#include <time.h>

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

namespace Test
{
	/// \brief XML output handler.
	///

	class XMLOutput : public Test::CollectorOutput
	{
	public:
		int tab_level;
		std::vector<const char*> open_tags;
		void tab_it(std::ostream& os)
		{
			for (int i = 0; i < tab_level + (int)open_tags.size() - 1; i++)
				os << "\t";
		}
		void open_tag(std::ostream& os, const char* tag, bool rc = true)
		{
			open_tags.push_back(tag);
			tab_it(os);
			os << "<" << tag << ">";
			if (rc)
				os << "\n";
		}
		template <class T>
		void write_tagged_value(std::ostream& os, const char* tag, const T& value)
		{
			open_tag(os, tag, false);
			os << escape(value);
			os << "</" << open_tags.back() << ">\n";
			open_tags.pop_back();
		}
		template <class T>
		void open_tag(std::ostream& os, const char* tag, const char* param, const T& param_value)
		{
			open_tags.push_back(tag);
			tab_it(os);
			os << "<" << tag << ' ' << param << '=' << '\"' << escape(param_value) << "\">\n";
		}
		void close_tag(std::ostream& os, const char* tag)
		{
			while (true)
			{
				tab_it(os);
				os << "</" << open_tags.back() << ">\n";
				bool done = strcmp(open_tags.back(), tag) == 0;
				open_tags.pop_back();
				if (done)
					break;
			}
		}

		XMLOutput(std::ostream& stream = std::cout)
			: _stream(stream)
		{

		}

		void generate(std::ostream& os, bool incl_ok_tests, const std::string& name, clock_t real_start_time)
		{
			os << "<?xml version=\"1.0\" encoding='ISO-8859-1' standalone='yes' ?>\n";
			tab_level = 0;
			open_tag(os, "TestRun");
			open_tag(os, "FailedTests");
			try
			{
				int id = 0;
				for (auto suit = _suites.begin(); suit != _suites.end(); ++suit)
				{
					if (suit->_errors > 0)
					{
						for (auto test = suit->_tests.begin(); test != suit->_tests.end(); ++test)
						{
							id++;
							if (!test->_success)
							{
								open_tag(os, "FailedTest", "id", id);
								write_tagged_value(os, "Name", suit->_name + "::" + test->_name);
								if (test->_sources.size() > 0)
								{
									write_tagged_value(os, "FailureType", "Assertion");
									open_tag(os, "Location");
									write_tagged_value(os, "File", test->_sources.front().file());
									write_tagged_value(os, "Line", test->_sources.front().line());
									close_tag(os, "Location");
									write_tagged_value(os, "Message", test->_sources.front().message());
								}
								else
								{
									write_tagged_value(os, "FailureType", "Exception");
									open_tag(os, "Location");
									write_tagged_value(os, "File", "UNKNOWN");
									write_tagged_value(os, "Line", 0);
									close_tag(os, "Location");
									write_tagged_value(os, "Message", "UNDEFINED (Was an exception thrown from the test?)");
								}
								write_tagged_value(os, "Time", test->_time);
								close_tag(os, "FailedTest");
							}
						}
					}
					else
					{
						id += (int)suit->_tests.size();
					}
				}
				close_tag(os, "FailedTests");

				if (incl_ok_tests)
				{
					open_tag(os, "SuccessfulTests");
					id = 0;
					for (auto suit = _suites.begin(); suit != _suites.end(); ++suit)
					{
						for (auto test = suit->_tests.begin(); test != suit->_tests.end(); ++test)
						{
							id++;
							if (test->_success)
							{
								open_tag(os, "Test", "id", id);
								write_tagged_value(os, "Name", suit->_name + "::" + test->_name);
								write_tagged_value(os, "Time", test->_time);
								close_tag(os, "Test");
							}
						}
					}
					close_tag(os, "SuccessfulTests");
				}
			}
			catch (...)
			{
			}

			open_tag(os, "Statistics");
			write_tagged_value(os, "Tests", _total_tests);
			write_tagged_value(os, "FailuresTotal", _total_errors);
			write_tagged_value(os, "Errors", 0);
			write_tagged_value(os, "Failures", _total_errors);
			write_tagged_value(os, "TotalElapsedTime", (clock() - real_start_time) / CLOCKS_PER_SEC);
			close_tag(os, "Statistics");
			close_tag(os, "TestRun");
		}
	private:
		std::ostream&   _stream;
		template <class T>
		typename T escape(const T& v) { return v; }
		std::string escape(const char* xml)
		{
			std::string s = "";
			for (const char* p = xml; p != nullptr && *p != 0; ++p)
			{
				switch (*p)
				{
				case '&':
					s += "&amp;";
					break;
				case '"':
					s += "&quot;";
					break;
				case '\'':
					s += "&apos;";
					break;
				case '<':
					s += "&lt;";
					break;
				case '>':
					s += "&gt;";
					break;
				default:
					s += *p;
				}
			}

			return s;
		}
		std::string escape(const std::string& xml)
		{
			return escape(xml.c_str());
		}
	};
} //namespace Test

#endif // CPPTEST_XMLOUTPUT_H
