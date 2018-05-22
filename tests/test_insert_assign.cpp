#define BOOST_TEST_MODULE "STLCachePolicyMRU"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(data) {
    cache<int,string,policy_none> c(10);

    c.insert_or_assign(1,"data1");
    BOOST_CHECK(c.fetch(1)=="data1");
    c.insert_or_assign(2,"data2");
    BOOST_CHECK(c.fetch(2)=="data2");
    c.insert_or_assign(3,"data3");
    BOOST_CHECK(c.fetch(3)=="data3");
    c.insert_or_assign(1,"data4");

    BOOST_CHECK(c.size()==3); //It shouldn't be bigger nor smaller

    BOOST_CHECK(c.fetch(1)=="data4");
    c.clear();
    BOOST_CHECK(c.size()==0);


    /*
    BOOST_CHECK(c.insert_or_assign(1,string("test"))); //Insert returns true for new entries

    BOOST_CHECK(!c.empty());
    BOOST_CHECK(c.size()==1);

    BOOST_CHECK(c.check(1));
    BOOST_CHECK(!c.check(2));

    BOOST_CHECK(!c.fetch(1).compare("test"));
    BOOST_REQUIRE_THROW(c.fetch(2),std::exception);

    c.erase(1);
    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.size()==0);
    BOOST_CHECK(!c.check(1));
    BOOST_REQUIRE_THROW(c.fetch(1),exception_invalid_key);

    BOOST_CHECK(c.insert_or_assign(1,string("test")));
    BOOST_CHECK(!c.insert_or_assign(1,string("newtest"))); //Insert returns false for existing entries and doesn't updates them
    BOOST_CHECK(c.insert_or_assign(2,string("testagain"))); //Insert returns true for new entries

    BOOST_CHECK(!c.empty());
    BOOST_CHECK(c.size()==2);

    BOOST_CHECK(!c.fetch(1).compare("test"));
    BOOST_CHECK(!c.fetch(2).compare("testagain"));

    c.clear();
    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.size()==0);
    BOOST_REQUIRE_THROW(c.fetch(1),exception_invalid_key);
    BOOST_REQUIRE_THROW(c.fetch(2),exception_invalid_key);*/
}

BOOST_AUTO_TEST_SUITE_END();