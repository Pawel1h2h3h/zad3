// main.cpp
#include "client.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

void printMenu() {
    std::cout << "\n--- FTP Client Menu ---\n"
              << " 1) Connect to server\n"
              << " 2) Login\n"
              << " 3) SITE command\n"
              << " 4) Last response\n"
              << " 5) System type\n"
              << " 6) File size\n"
              << " 7) Modification date\n"
              << " 8) Set option\n"
              << " 9) Clear callback\n"
              << "10) Change directory\n"
              << "11) Present working directory\n"
              << "12) Make directory\n"
              << "13) Remove directory\n"
              << "14) List directory (detailed)\n"
              << "15) List names\n"
              << "16) Change to parent\n"
              << "17) Download file\n"
              << "18) Upload file\n"
              << "19) Remove file\n"
              << "20) Rename file\n"
              << "21) Open data connection\n"
              << "22) Read data\n"
              << "23) Write data\n"
              << "24) Close data connection\n"
              << " 0) Quit\n"
              << "Choice: ";
}

int main() {
    FtpClient ftp;
    bool exit = false;

    while (!exit) {
        printMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::string host;
                std::cout << "Host: ";
                std::getline(std::cin, host);
                bool ok = ftp.connect(host);
                std::cout << (ok ? "Connected\n" : "Connect failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 2: {
                std::string user, pass;
                std::cout << "Username: ";
                std::getline(std::cin, user);
                std::cout << "Password: ";
                std::getline(std::cin, pass);
                bool ok = ftp.login(user, pass);
                std::cout << (ok ? "Login successful\n" : "Login failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 3: {
                std::string cmd;
                std::cout << "SITE command: ";
                std::getline(std::cin, cmd);
                bool ok = ftp.site(cmd);
                std::cout << (ok ? "SITE OK\n" : "SITE failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 4:
                std::cout << "Last response: " << ftp.lastResponse() << "\n";
                break;
            case 5:
                std::cout << "System type: " << ftp.sysType() << "\n";
                break;
            case 6: {
                std::string path;
                std::cout << "Remote path: ";
                std::getline(std::cin, path);
                long size = ftp.fileSize(path);
                if (size < 0)
                    std::cout << "Error: " << ftp.lastResponse() << "\n";
                else
                    std::cout << "Size: " << size << "\n";
                break;
            }
            case 7: {
                std::string path;
                std::cout << "Remote path: ";
                std::getline(std::cin, path);
                std::cout << "Modification date: " << ftp.modificationDate(path) << "\n";
                break;
            }
            case 8: {
                int opt;
                long val;
                std::cout << "Option code: ";
                std::cin >> opt;
                std::cout << "Value: ";
                std::cin >> val;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                bool ok = ftp.setOption(opt, val);
                std::cout << (ok ? "Option set\n" : "Option failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 9:
                std::cout << (ftp.clearCallback() ? "Callback cleared\n"
                                                 : "Clear callback failed: " + ftp.lastResponse() + "\n");
                break;
            case 10: {
                std::string path;
                std::cout << "Directory: ";
                std::getline(std::cin, path);
                bool ok = ftp.changeDirectory(path);
                std::cout << (ok ? "Changed DIR\n" : "Chdir failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 11:
                std::cout << "PWD: " << ftp.presentWorkingDirectory() << "\n";
                break;
            case 12: {
                std::string path;
                std::cout << "New directory name: ";
                std::getline(std::cin, path);
                bool ok = ftp.makeDirectory(path);
                std::cout << (ok ? "Directory created\n" : "Mkdir failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 13: {
                std::string path;
                std::cout << "Directory to remove: ";
                std::getline(std::cin, path);
                bool ok = ftp.removeDirectory(path);
                std::cout << (ok ? "Directory removed\n" : "Rmdir failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 14: {
                auto list = ftp.listDirectory();
                std::cout << "Directory listing:\n";
                for (const auto& f : list)
                    std::cout << "  " << f << "\n";
                break;
            }
            case 15: {
                auto names = ftp.listNames();
                std::cout << "Name list:\n";
                for (const auto& n : names)
                    std::cout << "  " << n << "\n";
                break;
            }
            case 16:
                std::cout << (ftp.changeToParent() ? "Moved up\n"
                                                  : "CDUP failed: " + ftp.lastResponse() + "\n");
                break;
            case 17: {
                std::string remote, local;
                std::cout << "Remote file: ";
                std::getline(std::cin, remote);
                std::cout << "Local file: ";
                std::getline(std::cin, local);
                bool ok = ftp.download(remote, local);
                std::cout << (ok ? "Downloaded\n" : "Download failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 18: {
                std::string local, remote;
                std::cout << "Local file: ";
                std::getline(std::cin, local);
                std::cout << "Remote file: ";
                std::getline(std::cin, remote);
                bool ok = ftp.upload(local, remote);
                std::cout << (ok ? "Uploaded\n" : "Upload failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 19: {
                std::string path;
                std::cout << "File to delete: ";
                std::getline(std::cin, path);
                bool ok = ftp.removeFile(path);
                std::cout << (ok ? "Deleted\n" : "Delete failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 20: {
                std::string from, to;
                std::cout << "Rename from: ";
                std::getline(std::cin, from);
                std::cout << "Rename to: ";
                std::getline(std::cin, to);
                bool ok = ftp.renameFile(from, to);
                std::cout << (ok ? "Renamed\n" : "Rename failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 21: {
                std::string path;
                int typ, mode;
                std::cout << "Path: ";
                std::getline(std::cin, path);
                std::cout << "Type (FTPLIB_FILE=1, FTPLIB_DIR=2): ";
                std::cin >> typ;
                std::cout << "Mode (FTPLIB_ASCII=0, FTPLIB_BINARY=1): ";
                std::cin >> mode;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                bool ok = ftp.open(path, typ, mode);
                std::cout << (ok ? "Data connection opened\n" : "Access failed: " + ftp.lastResponse() + "\n");
                break;
            }
            case 22: {
                int count;
                std::cout << "Bytes to read: ";
                std::cin >> count;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<char> buf(count);
                int r = ftp.read(buf.data(), count);
                std::cout << "Read " << r << " bytes\n";
                break;
            }
            case 23: {
                std::string data;
                std::cout << "Data to write: ";
                std::getline(std::cin, data);
                int w = ftp.write(data.data(), data.size());
                std::cout << "Written " << w << " bytes\n";
                break;
            }
            case 24:
                ftp.close();
                std::cout << "Data connection closed\n";
                break;

            case 0:
                exit = true;
                ftp.disconnect();
                break;
            default:
                std::cout << "Unknown option\n";
        }
    }

    return 0;
}