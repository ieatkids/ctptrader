#include <ThostFtdcMdApi.h>
#include <ThostFtdcTraderApi.h>

#include "ctpMG.hpp"
#include <base/defs.hpp>
#include <base/msg.hpp>
#include <gateway/ctpMG.hpp>
#include <service/dataService.hpp>
#include <util/channel.hpp>

namespace ctptrader::ctpMG {

void Spi::OnFrontConnected() {
  LOG_INFO("CtpMG::OnFrontConnected");
  CThostFtdcReqUserLoginField req{};
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, broker_id_.c_str());
  strcpy(req.UserID, user_id_.c_str());
  strcpy(req.Password, password_.c_str());
  if (api_->ReqUserLogin(&req, ++request_id_) == 0) {
    LOG_INFO("CtpMG::OnFrontConnected: Send ReqUserLogin success");
  } else {
    LOG_ERROR("CtpMG::OnFrontConnected: Send ReqUserLogin failed");
  }
}

void Spi::OnFrontDisconnected(int nReason) {
  LOG_INFO("CtpMG::OnFrontDisconnected: nReason = {}", nReason);
}

void Spi::OnHeartBeatWarning(int nTimeLapse) {
  LOG_INFO("CtpMG::OnHeartBeatWarning: nTimeLapse = {}", nTimeLapse);
}

void Spi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                         CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                         bool bIsLast) {
  if (pRspInfo && pRspInfo->ErrorID == 0) {
    LOG_INFO("CtpMG::OnRspUserLogin: Login success");
    char *ppInstrumentID[] = {"cu2311", "cu2312"};
    int count = 2;
    int res = api_->SubscribeMarketData(ppInstrumentID, count);
    if (res == 0) {
      LOG_INFO(
          "CtpMG::OnRspUserLogin: Send SubscribeMarketData request success");
    } else {
      LOG_ERROR(
          "CtpMG::OnRspUserLogin: Send SubscribeMarketData request failed");
    }
  } else {
    LOG_ERROR("CtpMG::OnRspUserLogin: Login failed");
  }
}

void Spi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                          CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                          bool bIsLast) {
  LOG_INFO("CtpMG::OnRspUserLogout: Logout success");
}

void Spi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                     bool bIsLast) {
  LOG_ERROR("CtpMG::OnRspError: ErrorID = {}, ErrorMsg = {}", pRspInfo->ErrorID,
            pRspInfo->ErrorMsg);
}

void Spi::OnRspSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  LOG_INFO("CtpMG::OnRspSubMarketData: InstrumentID = {}",
           pSpecificInstrument->InstrumentID);
}

void Spi::OnRspUnSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  LOG_INFO("CtpMG::OnRspUnSubMarketData: InstrumentID = {}",
           pSpecificInstrument->InstrumentID);
}

void Spi::OnRspSubForQuoteRsp(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  LOG_INFO("CtpMG::OnRspSubForQuoteRsp: InstrumentID = {}",
           pSpecificInstrument->InstrumentID);
}

void Spi::OnRspUnSubForQuoteRsp(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  LOG_INFO("CtpMG::OnRspUnSubForQuoteRsp: InstrumentID = {}",
           pSpecificInstrument->InstrumentID);
}

void Spi::OnRtnDepthMarketData(
    CThostFtdcDepthMarketDataField *pDepthMarketData) {
  LOG_INFO("MdSpi::OnRtnDepthMarketData: InstrumentID = {}, UpdateTime = {}, "
           "LastPrice = {}, UpdateMillisec = {}",
           pDepthMarketData->InstrumentID, pDepthMarketData->UpdateTime,
           pDepthMarketData->LastPrice, pDepthMarketData->UpdateMillisec);
}

void Spi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
  LOG_INFO("MdSpi::OnRtnForQuoteRsp: InstrumentID = {}, ForQuoteTime = {}",
           pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteTime);
}

} // namespace ctptrader::ctpMG
