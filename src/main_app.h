#ifndef __MAIN_APP_H__
#define __MAIN_APP_H__

#include <boost/beast.hpp>

#include <thread>
#include <vector>

namespace WsBinance {
class MainApp {
public:
  using io_context = boost::beast::net::io_context;

  /*!
   * MainApp ctor
   * @param threadNums Specify number of thread using to run application
   */
  explicit MainApp(size_t threadNums = 1);

  /*!
   * MainApp dtor
   * Try to call teardown on ExecutionContext, wait util it finish and return
   */
  ~MainApp();

  /// start application
  void run();

  void wait();

  /*!
   * Stop the execution of MainApp
   * - Remove all network handlers
   * - Clear message queue
   * - Wait util all of worker threads finish, then return
   */
  void stop();

  io_context &getContext();

private:
  io_context ctx_;
  size_t threadNum_;
  std::vector<std::thread> workers_;
}; // class MainApp
} // namespace WsBinance

#endif //__MAIN_APP_H__
