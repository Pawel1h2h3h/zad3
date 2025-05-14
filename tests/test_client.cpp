#include <catch2/catch_test_macros.hpp>


#include "client.hpp"

TEST_CASE("FtpClient::connect i disconnect", "[ftp]") {
    FtpClient client;

    SECTION("Nieprawidłowy host") {
        REQUIRE_FALSE(client.connect("invalid.host"));
    }

    SECTION("Prawidłowy host (lokalny serwer FTP wymagany)") {
        // Wymaga uruchomionego serwera FTP na localhost
        // REQUIRE(client.connect("127.0.0.1"));
    }
}

TEST_CASE("FtpClient::login", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");

    SECTION("Złe dane logowania") {
        REQUIRE_FALSE(client.login("wrong", "wrong"));
    }

    SECTION("Dobre dane logowania (wymaga serwera FTP z użytkownikiem test/test)") {
        // REQUIRE(client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu"));
    }
}

TEST_CASE("FtpClient::lastResponse", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");

    SECTION("Bez logowania") {
        std::string response = client.lastResponse();
        REQUIRE(!response.empty());
    }

    SECTION("Po logowaniu (jeśli serwer pozwala)") {
        client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");
        REQUIRE(client.lastResponse().find("230") != std::string::npos);
    }
}

TEST_CASE("FtpClient::site", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");
    client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");

    SECTION("Nieznana komenda SITE") {
        REQUIRE_FALSE(client.site("FOOBAR"));
    }

    SECTION("Prawidłowa komenda SITE (jeśli obsługiwana)") {
        // REQUIRE(client.site("HELP"));
    }
}

TEST_CASE("FtpClient::presentWorkingDirectory", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");
    client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");

    std::string pwd = client.presentWorkingDirectory();

    REQUIRE(!pwd.empty());
    REQUIRE(pwd[0] == '/');
}

TEST_CASE("FtpClient::changeDirectory i changeToParent", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");
    client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");

    SECTION("Zmieniam do katalogu /tmp (jeśli istnieje)") {
        client.changeDirectory("/tmp");
        REQUIRE(client.presentWorkingDirectory().find("/tmp") != std::string::npos);
    }

    SECTION("Powrót do katalogu nadrzędnego") {
        client.changeToParent();
        REQUIRE(client.presentWorkingDirectory() != "/tmp");
    }
}

TEST_CASE("FtpClient::makeDirectory i removeDirectory", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");
    client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");

    std::string dirname = "test_dir";

    SECTION("Utwórz katalog") {
        REQUIRE(client.makeDirectory(dirname));
    }

    SECTION("Usuń katalog") {
        client.makeDirectory(dirname);
        REQUIRE(client.removeDirectory(dirname));
    }
}

TEST_CASE("FtpClient::fileSize i modificationDate", "[ftp]") {
    FtpClient client;
    client.connect("ftp.dlptest.com");
    client.login("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");

    std::string file = "test.txt";

    SECTION("Plik nie istnieje") {
        REQUIRE(client.fileSize(file) == -1);
        REQUIRE(client.modificationDate(file).empty());
    }

    // SECTION("Plik istnieje") {
    //     REQUIRE(client.fileSize("existing.txt") > 0);
    //     REQUIRE(!client.modificationDate("existing.txt").empty());
    // }
}