// Mini Project Database Handler.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <optional>
#include <vector>

#include "databaseClass.h"

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif 

#define ASIO_STANDALONE


#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


std::vector<char> ASIOReadTCP(asio::ip::tcp::socket& socket);
int convertToInt(std::vector<char> buff);

asio::error_code ec;
asio::io_context context;

asio::ip::tcp::socket Socket(context);

int main()
{
    try
    {
        asio::ip::tcp::endpoint endpoint(asio::ip::make_address("192.168.33.90", ec), 8888);
        Socket.connect(endpoint, ec);
        
        if (!ec)
        {
            std::cout << "connected!" << std::endl;
        }
        else
        {
            std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
            exit(-1);
        }

        auto evmIDData = ASIOReadTCP(Socket);
        
        databaseClass* db;
        if (evmIDData[0]!='#')
        {
            int evmID = convertToInt(evmIDData);
            db = new databaseClass(evmID);
            db->show_party_count();
        }
        else
            exit(-1);

        while (true)
        {
            auto data = ASIOReadTCP(Socket);
            if (data[0]!='#' && db)
            {
                for(char c : data)
                    if(c >= 65 && c <= 90)
                        db->insert_into_table(c);

                db->show_party_count();
            }
        }
    }
    catch (const sql::SQLException& e)
    {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        if (e.getErrorCode() == 1047)
        {
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }
        std::cin.get();
        return EXIT_FAILURE;
    }
    catch (std::runtime_error& e)
    {
        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    return 0;
}

std::vector<char> ASIOReadTCP(asio::ip::tcp::socket& socket)
{
    asio::error_code ec;
    if (socket.is_open())
    {
        socket.wait(socket.wait_read);
        size_t bytes = socket.available();
        std::cout << "Bytes Available: " << bytes << std::endl;

        std::vector<char> vbuff(10);

        if (bytes > 0)
        {
            vbuff.resize(bytes);
            socket.read_some(asio::buffer(vbuff.data(), vbuff.size()), ec);
            for(auto c : vbuff)
                std::cout << c;
            std::cout << std::endl;
            return vbuff;
        }
        vbuff.push_back('#');
        return vbuff;
    }
}

int convertToInt(std::vector<char> buff)
{
    int num = 0;
    for (auto c : buff)
    {
        if (c >= 48 && c <= 57)
            num += c - 48;
    }
    return num;
}