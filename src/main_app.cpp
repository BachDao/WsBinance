#include "main_app.h"

#include <boost/asio.hpp>

using namespace WsBinance;

MainApp::MainApp(size_t threadNums) : threadNum_(threadNums) {}

MainApp::~MainApp() {
  for (auto &w : workers_) {
    if (w.joinable())
      w.join();
  }
}

MainApp::io_context &MainApp::getContext() { return ctx_; }

void MainApp::run() {
  for (int i = 0; i < threadNum_; ++i) {
    workers_.push_back(std::thread([this] { ctx_.run(); }));
  }
}

void MainApp::stop() { ctx_.stop(); }

void MainApp::wait() {
  for (auto &w : workers_) {
    if (w.joinable())
      w.join();
  }
}
