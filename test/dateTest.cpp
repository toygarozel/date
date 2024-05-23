#include "date.hpp"

#include <gtest/gtest.h>

#include <numeric>

TEST(DateTest, Ctors)
{
    using date::Date;

    Date d1;
    ASSERT_EQ(1, d1.get_month_day());
    ASSERT_EQ(1, d1.get_month());
    ASSERT_EQ(Date::year_base, d1.get_year());

    Date d2{23, 4, 1920};
    ASSERT_EQ(23, d2.get_month_day());
    ASSERT_EQ(4, d2.get_month());
    ASSERT_EQ(1920, d2.get_year());

    Date d3{"19/05/1919"};
    ASSERT_EQ(19, d3.get_month_day());
    ASSERT_EQ(5, d3.get_month());
    ASSERT_EQ(1919, d3.get_year());

    //Date d4{std::time(nullptr)};
    //ASSERT_EQ(Date(24, 5, 2024), d4); // today's date
}

TEST(DateTest, CompareOperators)
{
    date::Date yesterday{22, 4, 2024};
    date::Date today    {23, 4, 2024};
    date::Date tomorrow {24, 4, 2024};
    const date::Date nisan23{23, 4, 2024};

    ASSERT_TRUE(yesterday < today);
    ASSERT_TRUE(today > yesterday);
    ASSERT_TRUE(today == nisan23);
}

TEST(DateTest, CalculateYearDay)
{
    date::Date date {23, 4, 2024};
    constexpr int expected = 31 + 29 + 31 + 23;
    ASSERT_EQ(expected, date.get_year_day());
}

TEST(DateTest, MemberModifierOperators)
{
    using date::Date;
    
    Date tempDate;
    const Date nisan23{23, 4, 2024};
    const Date nisan24{24, 4, 2024};

    // Pre and Post increment/decrement
    tempDate = nisan23;
    ASSERT_EQ(++tempDate, nisan24);

    tempDate = nisan23;
    ASSERT_NE(tempDate++, nisan24);

    tempDate = nisan24;
    ASSERT_EQ(--tempDate, nisan23);
    
    tempDate = nisan24;
    ASSERT_NE(tempDate--, nisan23);

    // New year
    tempDate.set(31, 12, 2023);
    tempDate++;
    ASSERT_EQ(tempDate, Date(1, 1, 2024));

    tempDate.set(1, 1, 2024);
    tempDate--;
    ASSERT_EQ(tempDate, Date(31, 12, 2023));

    // Not existed date
    ASSERT_ANY_THROW(tempDate.set(29, 2, 2023));

    // Existed date, 29th Feb
    ASSERT_NO_THROW(tempDate.set(29, 2, 2024));

    tempDate = nisan23;
    tempDate += 7;
    ASSERT_EQ(tempDate, Date(30, 4, 2024));

    tempDate -= 14;
    ASSERT_EQ(tempDate, Date(16, 4, 2024));
}

TEST(DateTest, GlobalModifierOperators)
{
    date::Date date1{11, 11, 2011};
    date::Date date2{21, 11, 2011};

    ASSERT_EQ(date2 - date1, 10);
    ASSERT_EQ(date1 + 10, date2);
    ASSERT_EQ(10 + date1, date2);
}


TEST(DateTest, IsLeap)
{
    ASSERT_TRUE(date::Date::isleap(2020));
    ASSERT_FALSE(date::Date::isleap(2021));
}

TEST(DateTest, WeekdayOperators)
{
    using enum date::Date::Weekday;
    
    auto day = Friday;
    
    ASSERT_EQ(++day, Saturday);
    day++;
    ASSERT_EQ(day, Sunday);

    day = Monday;
    ASSERT_EQ(--day, Sunday);
    day--;
    ASSERT_EQ(day, Saturday);
}

TEST(DateTest, LessThanBaseYear)
{
    date::Date date{6, 4, 1453};

    ASSERT_EQ(date::Date(6,4,date::Date::year_base), date);
}


TEST(DateTest, GreaterThanLimitYear)
{
    date::Date date{5, 5, 3010};

    ASSERT_EQ(date::Date(5,5,date::Date::year_limit), date);
}