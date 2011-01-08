#ifndef EBITEN_UTIL_CHANNEL_HPP
#define EBITEN_UTIL_CHANNEL_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/thread.hpp>

namespace ebiten {
namespace util {

template<class Item>
class channel : private boost::noncopyable {
private:
  // TODO: buffer size
  boost::optional<Item> buffer;
  mutable boost::mutex mutex;
  boost::condition_variable_any condition;
public:
  void
  send(Item item) {
    boost::mutex::scoped_lock lock(this->mutex);
    while (this->buffer) {
      this->condition.wait(lock);
    }
    this->buffer = item;
    this->condition.notify_all();
  }
  Item
  receive() {
    boost::mutex::scoped_lock lock(this->mutex);
    while (!this->buffer) {
      this->condition.wait(lock);
    }
    auto item = *(this->buffer);
    this->buffer = boost::none;
    this->condition.notify_all();
    return item;
  }
  bool
  is_receivable() const {
    boost::mutex::scoped_lock lock(this->mutex);
    return this->buffer;
  }
};

}
}

#ifdef EBITEN_TEST

#include <boost/test/unit_test.hpp>

namespace ebiten {
namespace util {

BOOST_AUTO_TEST_CASE(test_channel) {
  channel<int> ch;
  std::vector<int> results;
  boost::thread producer([&]{
      ch.send(1);
      ch.send(2);
      ch.send(3);
      ch.send(4);
      ch.send(5);
    });
  boost::thread consumer([&]{
      results.push_back(ch.receive());
      results.push_back(ch.receive());
      results.push_back(ch.receive());
      results.push_back(ch.receive());
      results.push_back(ch.receive());
    });
  producer.join();
  consumer.join();
  BOOST_REQUIRE_EQUAL(5, results.size());
  BOOST_CHECK_EQUAL(1, results[0]);
  BOOST_CHECK_EQUAL(2, results[1]);
  BOOST_CHECK_EQUAL(3, results[2]);
  BOOST_CHECK_EQUAL(4, results[3]);
  BOOST_CHECK_EQUAL(5, results[4]);
}

BOOST_AUTO_TEST_CASE(test_channel_is_receivable) {
  channel<int> ch;
  std::vector<int> results;
  BOOST_CHECK_EQUAL(false, ch.is_receivable());
  ch.send(1);
  BOOST_CHECK_EQUAL(true, ch.is_receivable());
  ch.receive();
  BOOST_CHECK_EQUAL(false, ch.is_receivable());
}

}
}

#endif

#endif
