// ---
//
// $Id: time.cpp,v 1.4 2008/07/15 20:33:31 hartwork Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
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

#if (defined(__WIN32__) || defined(WIN32))
# include "winconfig.h"
#else
# include "config.h"
#endif 

#include "missing.h"
#include "cpptest-time.h"
#include <chrono>

using namespace std;

namespace Test
{
	namespace
	{
		const unsigned int UsecPerSec = 1000000;
		
	} // anonymous namespace
	
	/// Constructs a time object with zeroed time.
	///
	Time::Time()
	:	duration_(0)
	{}

	Time::Time(std::chrono::nanoseconds dur)
	:	duration_(dur)
	{}

	
	/// \return Seconds.
	///
	unsigned int
	Time::seconds() const
	{
		return (unsigned int)(duration_.count() / 1000000000);
	}
	
	/// \return Micro-seconds.
	///
	unsigned int
	Time::microseconds() const
	{
		return (unsigned int)((duration_.count() % 1000000000) / 1000.0 + 0.5);
	}
	
	/// \return The current time.
	///
	Time
	Time::current()
	{
		std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
		return Time(std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()));
	}
	
	/// \relates Time
	///
	/// Computes the time elapsed between two time values.
	///
	/// \param t1 Left-hand time, should be greater than \a t2.
	/// \param t2 Right-hand time, should be less than \a t1.
	///
	/// \return Computed time value.
	///
	Time
	operator-(const Time& t1, const Time& t2)
	{
		return Time(t1.duration_ - t2.duration_);
	}
	
	/// \relates Time
	///
	/// Adds two time values.
	///
	/// \param t1 Left-hand time.
	/// \param t2 Right-hand time.
	///
	/// \return Computed time value.
	///
	Time
	operator+(const Time& t1, const Time& t2)
	{
		return Time(t1.duration_ + t2.duration_);
	}
	
	/// \relates Time
	///
	/// Outputs a time to an output stream.
	///
	/// \param os Output stream to write to.
	/// \param t  %Time to output.
	///
	/// \return A reference to the given output stream.
	///
	ostream&
	operator<<(ostream& os, const Time& t)
	{
		int old_fill(os.fill());
		std::streamsize old_width(os.width());

		os << t.seconds() << '.';
		os.fill('0');
		os.width(6);
		os << t.microseconds();

		os.fill(old_fill);
		os.width(old_width);

		return os;
	}
	
} // namespace Test

