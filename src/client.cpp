// client.cpp
#include "client.hpp"
#include <fstream>
#include <iostream>

FtpClient::FtpClient()
: ctrl_(nullptr)
, data_(nullptr)
{
    FtpInit();
}

FtpClient::~FtpClient()
{
    if (data_) {
        FtpClose(data_);
        data_ = nullptr;
    }
    if (ctrl_) {
        FtpQuit(ctrl_);
        ctrl_ = nullptr;
    }
}

bool FtpClient::connect(const std::string& host) {
    return FtpConnect(host.c_str(), &ctrl_) != 0;
}

bool FtpClient::login(const std::string& user, const std::string& pass) {
    return FtpLogin(user.c_str(), pass.c_str(), ctrl_) != 0;
}

void FtpClient::disconnect() {
    if (ctrl_) {
        FtpQuit(ctrl_);
        ctrl_ = nullptr;
    }
}

std::string FtpClient::lastResponse() const {
    char* resp = FtpLastResponse(ctrl_);
    return resp ? std::string(resp) : std::string();
}

bool FtpClient::site(const std::string& command) {
    return FtpSite(command.c_str(), ctrl_) != 0;
}

std::string FtpClient::sysType() const {
    char buf[128] = {0};
    return FtpSysType(buf, sizeof(buf), ctrl_) ? std::string(buf) : std::string();
}

long FtpClient::fileSize(const std::string& path) const {
    unsigned int size = 0;
    if (FtpSize(path.c_str(), &size, FTPLIB_BINARY, ctrl_))
        return static_cast<long>(size);
    return -1;
}

std::string FtpClient::modificationDate(const std::string& path) const {
    char buf[128] = {0};
    return FtpModDate(path.c_str(), buf, sizeof(buf), ctrl_) ? std::string(buf) : std::string();
}

bool FtpClient::setOption(int opt, long val) {
    return FtpOptions(opt, val, ctrl_) != 0;
}

bool FtpClient::setCallback(const FtpCallbackOptions* opt) {
    return FtpSetCallback(opt, ctrl_) != 0;
}

bool FtpClient::clearCallback() {
    return FtpClearCallback(ctrl_) != 0;
}

std::vector<std::string> FtpClient::listDirectory(const std::string& path) {
    const char* tmp = "ftp_listing.txt";
    std::vector<std::string> out;
    if (FtpDir(tmp, path.c_str(), ctrl_) == 0)
        return out;
    std::ifstream in(tmp);
    std::string line;
    while (std::getline(in, line))
        out.push_back(line);
    return out;
}

std::vector<std::string> FtpClient::listNames(const std::string& path) {
    const char* tmp = "ftp_nlst.txt";
    std::vector<std::string> out;
    if (FtpNlst(tmp, path.c_str(), ctrl_) == 0)
        return out;
    std::ifstream in(tmp);
    std::string line;
    while (std::getline(in, line))
        out.push_back(line);
    return out;
}

bool FtpClient::changeDirectory(const std::string& path) {
    return FtpChdir(path.c_str(), ctrl_) != 0;
}

bool FtpClient::changeToParent() {
    return FtpCDUp(ctrl_) != 0;
}

std::string FtpClient::presentWorkingDirectory() const {
    char buf[256] = {0};
    return FtpPwd(buf, sizeof(buf), ctrl_) ? std::string(buf) : std::string();
}

bool FtpClient::makeDirectory(const std::string& path) {
    return FtpMkdir(path.c_str(), ctrl_) != 0;
}

bool FtpClient::removeDirectory(const std::string& path) {
    return FtpRmdir(path.c_str(), ctrl_) != 0;
}

bool FtpClient::download(const std::string& remotePath,
                         const std::string& localPath,
                         int mode)
{
    return FtpGet(remotePath.c_str(), localPath.c_str(), mode, ctrl_) != 0;
}

bool FtpClient::upload(const std::string& localPath,
                       const std::string& remotePath,
                       int mode)
{
    return FtpPut(localPath.c_str(), remotePath.c_str(), mode, ctrl_) != 0;
}

bool FtpClient::removeFile(const std::string& path) {
    return FtpDelete(path.c_str(), ctrl_) != 0;
}

bool FtpClient::renameFile(const std::string& from, const std::string& to) {
    return FtpRename(from.c_str(), to.c_str(), ctrl_) != 0;
}

bool FtpClient::open(const std::string& path, int typ, int mode) {
    if (data_) {
        FtpClose(data_);
        data_ = nullptr;
    }
    int result = FtpAccess(path.c_str(), typ, mode, ctrl_, &data_);
    return (result != 0 && data_ != nullptr);
}

int FtpClient::read(void* buf, int count) {
    return data_ ? FtpRead(buf, count, data_) : 0;
}

int FtpClient::write(const void* buf, int count) {
    return data_ ? FtpWrite(buf, count, data_) : 0;
}

void FtpClient::close() {
    if (data_) {
        FtpClose(data_);
        data_ = nullptr;
    }
}