#ifndef DATE_H
#define DATE_H

#include <iosfwd>
#include <ctime>
#include <array>
#include <string>
#include <unordered_map>

namespace date {
	class Date {
	public:
		static constexpr int year_base = 1900;  //1
		static constexpr int year_limit = 2199;  //1
		static constexpr int random_min_year = 1940;  //2
		static constexpr int random_max_year = 2020;  //3
		
		enum class Weekday { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday }; //4

		inline static const std::unordered_map<Weekday, std::string> weekday_table {
			{Weekday::Sunday, "Sunday"},
			{Weekday::Monday, "Monday"},
			{Weekday::Tuesday, "Tuesday"},
			{Weekday::Wednesday, "Wednesday"},
			{Weekday::Thursday, "Thursday"},
			{Weekday::Friday, "Friday"},
			{Weekday::Saturday, "Saturday"}
		};

		inline static const std::unordered_map<int, std::string> month_table {
			{1, "Ocak"},
			{2, "Subat"},
			{3, "Mart"},
			{4, "Nisan"},
			{5, "Mayis"},
			{6, "Haziran"},
			{7, "Temmuz"},
			{8, "Agustos"},
			{9, "Eylul"},
			{10, "Ekim"},
			{11, "Kasim"},
			{12, "Aralik"}
		};

		Date() noexcept; //5 
		Date(int d, int m, int y);  //6

		explicit Date(const char* p);  //7
		explicit Date(std::time_t timer); //8

		int get_month_day()const noexcept { return day; } //9
		int get_month()const noexcept { return month; } //10
		int get_year()const noexcept { return year; } //11
		int get_year_day()const; //12
		Weekday get_week_day()const; //13
		int get_days_in_month() const;
		int get_days_in_month(int m) const;
		static int get_days_in_month(int m, int y);

		Date& set_month_day(int day); //14
		Date& set_month(int month); //15
		Date& set_year(int year); //16
		Date& set(int day, int mon, int year); //17

		Date operator-(int day)const; //18
		Date& operator+=(int day); //19
		Date& operator-=(int day); //20
		Date& operator++(); //21
		Date operator++(int); //22
		Date& operator--(); //23
		Date operator--(int); //24

		static Date random_date(); //25
		static constexpr bool isleap(int y); //26
		
		int to_julian_day() const;
		static Date from_julian_day(int jd);

		friend std::ostream& operator<<(std::ostream& os, const Date& date); //31
		friend std::istream& operator>>(std::istream& is, Date& date); //32

	private:
		int year;
		int month;
		int day;
	};

	bool operator<(const Date&, const Date&); //27
	bool operator==(const Date&, const Date&); //27
	bool operator<=(const Date&, const Date&); //27
	bool operator>(const Date&, const Date&); //27
	bool operator>=(const Date&, const Date&); //27
	bool operator!=(const Date&, const Date&); //27

	int operator-(const Date& d1, const Date& d2); //28
	Date operator+(const Date& date, int n); //29
	Date operator+(int n, const Date&); //29
	Date::Weekday& operator++(Date::Weekday& r); //30
	Date::Weekday operator++(Date::Weekday& r, int); //30
	Date::Weekday& operator--(Date::Weekday& r); //30
	Date::Weekday operator--(Date::Weekday& r, int); //30}

} // namespace date

#endif