#ifndef CPPTEST_SIMPLECOLLECTOR_H
#define CPPTEST_SIMPLECOLLECTOR_H

/**
This is extension for the test to allow XML output
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
	class SimpleLogCollector : public Test::Output
	{
	private:
		enum Method {
			enum_finished,
			enum_suite_start,
			enum_suite_end,
			enum_test_start,
			enum_test_end,
			enum_assertment,
		};
		struct BaseMessage
		{
			Method method_;
		};
		struct FinishedMessage : public BaseMessage
		{
			int tests_;
			Test::Time time_;
			FinishedMessage(int tests, const Test::Time& time)
				: time_(time.seconds(), time.microseconds())
			{
				method_ = enum_finished;
				tests_ = tests;
			}
		};
		struct SuiteStartMessage : public BaseMessage
		{
			int tests_;
			std::string name_;
			bool ok_;
			SuiteStartMessage(int tests, const std::string& name)
			{
				method_ = enum_suite_start;
				tests_ = tests;
				name_ = name;
				ok_ = true;
			}
		};
		struct SuiteEndMessage : public BaseMessage
		{
			int tests_;
			std::string name_;
			Test::Time time_;
			bool ok_;
			SuiteEndMessage(int tests, const std::string& name, bool ok,
				const Test::Time& time)
			{
				tests_ = tests;
				name_ = name;
				time_ = time;
				method_ = enum_suite_end;
				ok_ = ok;
			}
		};
		struct TestStartMessage : public BaseMessage
		{
			std::string name_;
			bool ok_;
			TestStartMessage(const std::string& name)
			{
				name_ = name;
				method_ = enum_test_start;
				ok_ = true;
			}
		};
		struct TestEndMessage : public BaseMessage
		{
			std::string name_;
			Test::Time time_;
			bool ok_;
			TestEndMessage(const std::string& name, bool ok,
				const Test::Time& time)
			{
				name_ = name;
				ok_ = ok;
				time_ = time;
				method_ = enum_test_end;
			}
		};
		struct AssertmentMessage : public BaseMessage
		{
			Test::Source source_;
			AssertmentMessage(const Test::Source& source)
			{
				method_ = enum_assertment;
				source_ = source;
			}
		};
		std::list<BaseMessage*> messages_;

		std::unique_ptr<Test::TextOutput> con_;
		std::list<BaseMessage*>::iterator current_suit;
		std::list<BaseMessage*>::iterator current_test;
		bool errors_;
	public:
		enum Mode
		{
			enum_write_all_output = 0,
			enum_when_failed_write_only_failures = 1
		};
		virtual void finished(int tests, const Test::Time& time)
		{
			if (con_)
				con_->finished(tests, time);

			messages_.push_back(new FinishedMessage(tests, time));
		}
		virtual void suite_start(int tests, const std::string& name)
		{
			if (con_)
				con_->suite_start(tests, name);

			messages_.push_back(new SuiteStartMessage(tests, name));
			current_suit = messages_.end();
			current_suit--;
		}
		virtual void suite_end(int tests, const std::string& name,
			const Test::Time& time)
		{
			if (con_)
				con_->suite_end(tests, name, time);

			bool ok = ((SuiteStartMessage*)(*current_suit))->ok_;
			messages_.push_back(new SuiteEndMessage(tests, name, ok, time));
		}
		virtual void test_start(const std::string& name)
		{
			if (con_)
				con_->test_start(name);

			messages_.push_back(new TestStartMessage(name));
			current_test = messages_.end();
			current_test--;
		}
		virtual void test_end(const std::string& name, bool ok,
			const Test::Time& time)
		{
			if (con_)
				con_->test_end(name, ok, time);

			if (!ok)
			{
				((TestStartMessage*)(*current_test))->ok_ = false;
				((SuiteStartMessage*)(*current_suit))->ok_ = false;
			}

			messages_.push_back(new TestEndMessage(name, ok, time));
			errors_ |= !ok;
		}

		virtual void assertment(const Test::Source& s)
		{
			if (con_)
				con_->assertment(s);

			messages_.push_back(new AssertmentMessage(s));
		}

		void playTo(Test::Output& output, enum Mode mode = enum_when_failed_write_only_failures)
		{
			bool skip_successfull = errors_ && mode == enum_when_failed_write_only_failures;
			int count = 0;
			for (auto it = messages_.begin(); it != messages_.end(); ++it)
			{
				BaseMessage* base = (*it);
				switch (base->method_)
				{
				case enum_finished:
					output.finished(count, static_cast<const FinishedMessage*>(base)->time_);
					break;
				case enum_suite_start:
					if (!skip_successfull || !static_cast<const SuiteStartMessage*>(base)->ok_)
					{
						output.suite_start(static_cast<const SuiteStartMessage*>(base)->tests_, static_cast<const SuiteStartMessage*>(base)->name_);
					}
					break;
				case enum_suite_end:
					if (!skip_successfull || !static_cast<const SuiteEndMessage*>(base)->ok_)
					{
						output.suite_end(static_cast<const SuiteEndMessage*>(base)->tests_, static_cast<const SuiteEndMessage*>(base)->name_,
							static_cast<const SuiteEndMessage*>(base)->time_);
					}
					break;
				case enum_test_start:
					if (!skip_successfull || !static_cast<const TestStartMessage*>(base)->ok_)
					{
						output.test_start(static_cast<const TestStartMessage*>(base)->name_);
					}
					break;
				case enum_test_end:
					if (!skip_successfull || !static_cast<const TestEndMessage*>(base)->ok_)
					{
						count++;
						output.test_end(static_cast<const TestEndMessage*>(base)->name_, static_cast<const TestEndMessage*>(base)->ok_,
							static_cast<const TestEndMessage*>(base)->time_);
					}
					break;
				case enum_assertment:
					output.assertment(static_cast<const AssertmentMessage*>(base)->source_);
					break;
				default:
					assert(0);
					exit(-1);
				}
			}
		}

		bool has_errors() const { return errors_; }

		SimpleLogCollector(bool writeToConsole)
		{
			if (writeToConsole)
			{
				con_.reset(new Test::TextOutput(Test::TextOutput::Verbose));
			}

			errors_ = false;
		}
	};

} // namespace Test

#endif // #ifndef CPPTEST_SIMPLECOLLECTOR_H

