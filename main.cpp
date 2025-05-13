#include <iostream>
#include <fstream>
#include "ftplib/src/ftplib.h"

int main() {
    netbuf* ctrl = nullptr;

    const char* host = "ftp.gnu.org";
    const char* user = "anonymous";
    const char* pass = "anonymous";

    if (!FtpConnect(host, &ctrl)) {
        std::cerr << "❌ Nie można połączyć się z serwerem FTP.\n";
        return 1;
    }

    if (!FtpLogin(user, pass, ctrl)) {
        std::cerr << "❌ Logowanie nieudane.\n";
        FtpQuit(ctrl);
        return 1;
    }

    if (!FtpDir("listing.txt", ".", ctrl)) {
        std::cerr << "❌ Nie udało się pobrać listy plików.\n";
    } else {
        std::cout << "📄 Lista plików:\n";
        std::ifstream file("listing.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
    }

    FtpQuit(ctrl);
    return 0;
}