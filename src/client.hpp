// client.hpp
#ifndef FTPCLIENT_HPP
#define FTPCLIENT_HPP

#include <string>
#include <vector>
#include "ftplib.h"


class FtpClient {
public:
    FtpClient();
    ~FtpClient();

    // Połączenie
    bool connect(const std::string& host);
    bool login(const std::string& user, const std::string& pass);
    void disconnect();

    // Status / misc
    std::string lastResponse() const;
    bool site(const std::string& command);
    std::string sysType() const;
    long fileSize(const std::string& path) const;
    std::string modificationDate(const std::string& path) const;
    bool setOption(int opt, long val);
    bool setCallback(const FtpCallbackOptions* opt);
    bool clearCallback();

    // Katalogi
    std::vector<std::string> listDirectory(const std::string& path = ".");
    std::vector<std::string> listNames(const std::string& path = ".");
    bool changeDirectory(const std::string& path);
    bool changeToParent();
    std::string presentWorkingDirectory() const;
    bool makeDirectory(const std::string& path);
    bool removeDirectory(const std::string& path);

    // Transfer plik ↔ plik
    bool download(const std::string& remotePath,
                  const std::string& localPath,
                  int mode = FTPLIB_ASCII);
    bool upload(const std::string& localPath,
                const std::string& remotePath,
                int mode = FTPLIB_BINARY);
    bool removeFile(const std::string& path);
    bool renameFile(const std::string& from, const std::string& to);

    // Transfer plik ↔ program
    bool open(const std::string& path, int typ, int mode = FTPLIB_BINARY);
    int  read(void* buf, int count);
    int  write(const void* buf, int count);
    void close();

private:
    netbuf* ctrl_;   // kontrolne połączenie
    netbuf* data_;   // połączenie danych (dla FtpAccess())
};

#endif // FTPCLIENT_HPP