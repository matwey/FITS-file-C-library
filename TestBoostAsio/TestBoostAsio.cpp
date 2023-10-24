#include "TestBoostAsio.h"

//
// Тестовая программа для начала работы с BoostAsio.
// TCP-сервер на порту 8080 отправляет подключившимся клиентам "Hello, World!".
//

int main() 
{
    try 
    {
        boost::asio::io_service io;
        boost::asio::ip::tcp::acceptor acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

        while (true) 
        {
            boost::asio::ip::tcp::socket socket(io);
            acceptor.accept(socket);

            std::string message = "Hello, World!\n";
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e) 
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}