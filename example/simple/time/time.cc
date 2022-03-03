/**
 * @file time.cc
 * @author Sigma711 (sigma711 at foxmail dot com)
 * @brief  // TODO:
 * @date 2022-03-03
 *
 * @copyright Copyright (c) 2022 Sigma711
 *
 */

#include "time.h"

#include <stdio.h>
#include <sys/time.h>

TimeServer::TimeServer(const taotu::NetAddress& listen_address,
                       bool should_reuse_port)
    : server_(
          std::make_unique<taotu::Server>(listen_address, should_reuse_port)) {
  server_->SetConnectionCallback(std::bind(&TimeServer::OnConnectionCallback,
                                           this, std::placeholders::_1));
  server_->SetMessageCallback(
      std::bind(&TimeServer::OnMessageCallback, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
}

void TimeServer::Start() { server_->Start(); }

void TimeServer::OnConnectionCallback(taotu::Connecting& connection) {
  taotu::LOG(taotu::logger::kDebug,
             "EchoServer - (Ip(" + connection.GetLocalNetAddress().GetIp() +
                 "), Port(" +
                 std::to_string(connection.GetLocalNetAddress().GetPort()) +
                 ")) -> (Ip(" + connection.GetPeerNetAddress().GetIp() +
                 "), Port(" +
                 std::to_string(connection.GetPeerNetAddress().GetPort()) +
                 ")) - " + (connection.IsConnected() ? "UP." : "Down."));
  int64_t now_time = taotu::TimePoint{}.GetMicroseconds();
  time_t seconds = static_cast<time_t>(now_time / (1000 * 1000));
  struct tm tm_time;
  ::gmtime_r(&seconds, &tm_time);
  char buf[64] = {0};
  snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
           static_cast<int>(now_time % (1000 * 1000)));
  std::string data(buf);
  data += '\n';
  connection.Send(data);
  ::printf("%s", data.c_str());
}
void TimeServer::OnMessageCallback(taotu::Connecting& connection,
                                   taotu::IoBuffer* io_buffer,
                                   taotu::TimePoint time_point) {
  std::string message{io_buffer->RetrieveAllAsString()};
  taotu::LOG(taotu::logger::kDebug,
             "Fd(" + std::to_string(connection.Fd()) + ") is echoing " +
                 std::to_string(message.size()) + " bytes(" +
                 message.substr(0, message.size() - 1) + ") received at " +
                 std::to_string(time_point.GetMicroseconds()) + ".");
  int64_t now_time = time_point.GetMicroseconds();
  time_t seconds = static_cast<time_t>(now_time / (1000 * 1000));
  struct tm tm_time;
  ::gmtime_r(&seconds, &tm_time);
  char buf[64] = {0};
  snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
           static_cast<int>(now_time % (1000 * 1000)));
  std::string data(buf);
  data += '\n';
  connection.Send(data);
  ::printf("%s", data.c_str());
}
