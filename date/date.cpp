#include "date.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>

namespace {
	constexpr std::array<int, 12> monthDaysWithoutLeap{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	constexpr std::array<int, 12> monthDaysWithLeap	  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

}

namespace date {
	using Weekday = Date::Weekday;
	using enum Weekday;

	Date::Date() noexcept
	: year{ year_base }
	, month{1}
	, day{1}
	{}

	Date::Date(int d, int m, int y)
	: year{std::clamp(y, year_base, year_limit)}
	, month{std::clamp(m, 1, 12)}
	, day{std::clamp(d, 1, Date::get_days_in_month(month, year))}
	{}

	Date::Date(const char* p)
	{
		const std::string str{ p };

		const auto pos1 = str.find_first_of('/');
		const auto pos2 = str.find_last_of('/');

		if (pos1 == str.npos or pos2 == str.npos)
		{
			throw std::invalid_argument{ "Please enter date in format: dd/mm/yyyy" };
		}

		
		auto day = std::stoi(std::string{ str.begin(), str.begin() + pos1 });
		auto month = std::stoi(std::string{ str.begin() + pos1 + 1, str.begin() + pos2 });
		auto year = std::stoi(std::string{ str.begin() + pos2 + 1, str.end() });
		set(day, month, year);
	}

	Date::Date(std::time_t timer)
	{
		if (auto timeinfo = std::localtime(&timer); not timeinfo)
		{
			throw std::runtime_error("Failed to get local time");
		}
		else
		{
			set(timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
		}
	}

	// Algorithm: https://en.wikipedia.org/wiki/Zeller%27s_congruence
	Weekday Date::get_week_day()const
	{
		auto d = get_month_day();
		auto m = get_month();
		auto y = get_year();

		if(m < 3)
		{
			m += 12;
			y--;
		}

		int K = y / 100;
		int J = y / 100;
		int h = (d + 13*(m + 1)/5 + K + K/4 + J/4 + 5*J) % 7;

		return static_cast<Weekday>(h);
	}

	int Date::get_year_day()const
	{
		int total_days{get_month_day()};

		for (int m{1}; m < get_month(); ++m)
		{
			total_days += get_days_in_month(m);
		}

		return total_days;
	}

	int Date::get_days_in_month() const
	{
		return get_days_in_month(get_month());
	}

	int Date::get_days_in_month(int m) const
	{
		if(m < 1 or m > 12)
		{
			throw std::invalid_argument{"month value must be in [1-12] (which is " + std::to_string(m) + ')'};
		}
		return isleap(get_year()) ? monthDaysWithLeap[m - 1] : monthDaysWithoutLeap[m - 1];
	}

	int Date::get_days_in_month(int m, int y)
	{
		if (m < 1 or m > 12) throw std::invalid_argument{ "Month must be between 1-12" };
		return isleap(y) ? monthDaysWithLeap[m - 1] : monthDaysWithoutLeap[m - 1];
	}

	constexpr bool Date::isleap(int y)
	{
		return (y % 4 == 0) and ((y % 100 != 0) or (y % 400 == 0));
	}

	Date& Date::set_month_day(int d)
	{
		if (d > get_days_in_month() or d <= 0)
		{
			throw std::invalid_argument{"invalid month_day value, must be not exceed current month days"};
		}
		day = d;
		return *this;
	}

	Date& Date::set_month(int m)
	{
		if (m > 12 or m < 1)
		{
			throw std::invalid_argument{"invalid month value, must be in [1-12]"};
		}
		month = m;
		return *this;
	}

	Date& Date::set_year(int y)
	{
		year = y;
		return *this;
	}

	Date& Date::set(int d, int m, int y)
	{
		set_year(y);
		set_month(m);
		set_month_day(d);
		return *this;
	}

	Date Date::operator-(int day)const
	{
		return Date::from_julian_day(to_julian_day() - day);
	}

	Date& Date::operator+=(int day)
	{
		return *this = *this + day;
	}

	Date& Date::operator-=(int day)
	{
		return *this = *this - day;
	}

	Date& Date::operator--()
	{
		return *this -= 1;
	}

	Date Date::operator--(int)
	{
		const auto temp = *this;
		*this -= 1;
		return temp;
	}

	Date& Date::operator++()
	{
		return *this += 1;
	}

	Date Date::operator++(int)
	{
		const auto temp = *this;
		*this += 1;
		return temp;
	}

	Date Date::random_date()
	{
		std::random_device r;
		std::default_random_engine e1(r());
		
		std::uniform_int_distribution<int> random_year(random_min_year, random_max_year);
		std::uniform_int_distribution<int> random_month(1, 12);

		int year = random_year(e1);
		int month = random_month(e1);

		std::uniform_int_distribution<int> random_day(1, get_days_in_month(month, year));

		int day = random_day(e1);
		
		return Date{ day, month, year };
	}

	std::ostream& operator<<(std::ostream& os, const Date& date)
	{
		return os << date.get_month_day() << ' '
			<< Date::month_table.at(date.get_month()) << ' '
			<< date.get_year() << ' '
			<< Date::weekday_table.at(date.get_week_day());
	}

	std::istream& operator>>(std::istream& is, Date& date)
	{
		std::string str_date;
		
		is >> str_date;
		date = Date{ str_date.c_str()};
		
		return is;
	}

	bool operator<(const Date& lhs, const Date& rhs)
	{
		return lhs.get_year() < rhs.get_year() ? true : lhs.get_year_day() < rhs.get_year_day();
	}

	bool operator==(const Date& lhs, const Date& rhs)
	{
		return (lhs.get_year() == rhs.get_year()) and (lhs.get_year_day() == rhs.get_year_day());
	}

	bool operator<=(const Date& lhs, const Date& rhs)
	{
		return lhs < rhs or lhs == rhs;
	}

	bool operator>(const Date& lhs, const Date& rhs)
	{
		return rhs < lhs and lhs != rhs;
	}

	bool operator>=(const Date& lhs, const Date& rhs)
	{
		return rhs < lhs or lhs == rhs;
	}

	bool operator!=(const Date& lhs, const Date& rhs)
	{
		return !operator==(lhs, rhs);
	}

    int Date::to_julian_day() const
    {    
		int a = (14 - get_month()) / 12;
		int y = get_year() + 4800 - a;
		int m = get_month() + 12 * a - 3;
		return get_month_day() + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	}

    Date Date::from_julian_day(int jd)
    {
		int a = jd + 32044;
		int b = (4 * a + 3) / 146097;
		int c = a - (b * 146097) / 4;

		int d = (4 * c + 3) / 1461;
		int e = c - (1461 * d) / 4;
		int m = (5 * e + 2) / 153;

		int day = e - (153 * m + 2) / 5 + 1;
		int month = m + 3 - 12 * (m / 10);
		int year = b * 100 + d - 4800 + m / 10;

		return Date(day, month, year);
    }

    int operator-(const Date& d1, const Date& d2)
	{
		return d1.to_julian_day() - d2.to_julian_day();
	}

	Date operator+(const Date& date, int n)
	{
		return Date::from_julian_day(date.to_julian_day() + n);
	}
	
	Date operator+(int n, const Date& date)
	{
		return operator+(date, n);
	}

	Date::Weekday& operator++(Date::Weekday& r)
	{
		if (r == Saturday)
		{
			r = Sunday;
		}
		else
		{
			r = static_cast<Weekday>(std::to_underlying<Weekday>(r) + 1);
		}
		return r;
	}

	Date::Weekday operator++(Date::Weekday& r, int)
	{
		return operator++(r);
	}

	Date::Weekday& operator--(Date::Weekday& r)
	{
		if (r == Sunday)
		{
			r = Saturday;
		}
		else
		{
			r = static_cast<Weekday>(std::to_underlying<Weekday>(r) - 1);
		}
		return r;
	}

	Date::Weekday operator--(Date::Weekday& r, int)
	{
		return operator--(r);
	}

} // namespace date