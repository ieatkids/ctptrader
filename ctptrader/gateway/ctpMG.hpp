#pragma once

#include <ThostFtdcMdApi.h>
#include <ThostFtdcTraderApi.h>
#include <fmt/format.h>
#include <toml.hpp>

#include <base/defs.hpp>
#include <base/msg.hpp>
#include <service/dataService.hpp>
#include <util/channel.hpp>

namespace ctptrader::ctpMG {

class Spi : public CThostFtdcMdSpi {
public:
  Spi(CThostFtdcMdApi *api, const std::string &&broker_id,
      const std::string &&user_id, const std::string &&password,
      std::string_view shm_addr, std::vector<std::string> instruments)
      : api_(api), broker_id_(std::move(broker_id)),
        user_id_(std::move(user_id)), password_(std::move(password)),
        instruments_(std::move(instruments)), tx_(shm_addr) {}
  ~Spi() = default;

  void OnFrontConnected() override;
  void OnFrontDisconnected(int nReason) override;
  void OnHeartBeatWarning(int nTimeLapse) override;
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast) override;
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                       CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                       bool bIsLast) override;
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                  bool bIsLast) override;
  void
  OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                     CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                     bool bIsLast) override;
  void
  OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                       CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                       bool bIsLast) override;
  void
  OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast) override;
  void
  OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                        CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                        bool bIsLast) override;
  void OnRtnDepthMarketData(
      CThostFtdcDepthMarketDataField *pDepthMarketData) override;
  void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) override;

private:
  CThostFtdcMdApi *api_;
  const std::string broker_id_;
  const std::string user_id_;
  const std::string password_;
  const std::vector<std::string> instruments_;
  util::ShmSpscWriter<base::Depth, 1024> tx_;
  int request_id_{0};
};

inline void Start(const toml::table &config) {
  auto data_folder = config["data_folder"].value<std::string>();
  auto broker_id = config["broker_id"].value<std::string>();
  auto user_id = config["user_id"].value<std::string>();
  auto password = config["password"].value<std::string>();
  auto market_front = config["market_front"].value<std::string>();
  auto instruments = config["instruments"].as_array();
  std::vector<std::string> ins;
  for (auto &&ele : *instruments) {
    auto v = ele.value<std::string>();
    if (v.has_value()) {
      ins.emplace_back(v.value());
    }
  }
  LOG_INFO("CtpMG::Start");
  auto address = fmt::format("tcp://{}", market_front.value());
  auto *api = CThostFtdcMdApi::CreateFtdcMdApi();
  Spi spi(api, std::move(broker_id.value()), std::move(user_id.value()),
          std::move(password.value()), "shm_ctptrader_md", ins);
  api->RegisterSpi(&spi);
  api->RegisterFront((char *)address.c_str());
  api->Init();
  api->Join();
  api->Release();
}

} // namespace ctptrader::ctpMG
