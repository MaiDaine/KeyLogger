//
// Created by baptiste on 9/25/17.
//

#ifndef SPIDER_CONNECTION_HPP
# define SPIDER_CONNECTION_HPP

#include <boost/thread/mutex.hpp>
#include "Serial.hpp"

using boost::asio::ip::tcp;

class SSLConnection : public boost::enable_shared_from_this<SSLConnection>
{
  public:
    typedef boost::shared_ptr<SSLConnection> 			sharedConnectionPtr;
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> 	sslSocket;

    SSLConnection(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context)
      : _socket(io_service, context)
    {
    }

    sslSocket::lowest_layer_type& getSocket()
    {
      return _socket.lowest_layer();
    }

    template <typename Callback>
    void 	handShake(Callback callback) {
      _socket.async_handshake(boost::asio::ssl::stream_base::server, callback);
    }

    template <typename T, typename Callback>
    void write(T & data, Callback callback)
    {
      std::ostringstream	serializedData;
      std::string		fullData = Serial::serializeData(data);

      if (fullData == "ERROR") {
	callback(boost::asio::error::fault);
	return;
      }
      serializedData << std::setfill('0') << std::setw(HeaderSize) << fullData.size();
      std::string packet = serializedData.str() + fullData;
      _writeMutex.lock();
      boost::asio::async_write(_socket, boost::asio::buffer(packet),
			       [this, callback](const boost::system::error_code &e, const size_t &)
			       {
				 _writeMutex.unlock();
				 callback(e);
			       });
    }

    template <typename T>
    boost::system::error_code sync_write(T & data)
    {
      std::ostringstream            	serializedData;
      boost::system::error_code		e;
      std::string			fullData = Serial::serializeData(data);

      if (fullData == "ERROR")
	return boost::asio::error::fault;
      serializedData << std::setfill('0') << std::setw(HeaderSize) << fullData.size();
      std::string packet(serializedData.str() + fullData);

      _writeMutex.lock();
      boost::asio::write(_socket, boost::asio::buffer(packet), e);
      _writeMutex.unlock();

      return e;
    }

    template <typename T, typename Callback>
    void read(T & dataStruct, Callback callback)
    {
      boost::asio::async_read(_socket, boost::asio::buffer(_headerBuf, HeaderSize),
			      [this, &dataStruct, callback](const boost::system::error_code &e, const size_t &)
			      { handleHeader(e, dataStruct, callback); });
    }

  private:
    enum : unsigned int{ HeaderSize = 8 };

    template <typename T, typename Callback>
    void handleHeader(const boost::system::error_code& error,
		      T & dataStruct, Callback callback)
    {
      unsigned int bytesTransferred = 0;
      std::istringstream iss(_headerBuf);

      if (!error)
      {
	  if (!(iss >> bytesTransferred))
	  {
	    boost::system::error_code e(boost::asio::error::invalid_argument);
	    callback(e);
	    return ;
	  }
	  _packetData.resize(bytesTransferred);
	  boost::asio::async_read(_socket, boost::asio::buffer(_packetData),
				  [this, &dataStruct, callback](const boost::system::error_code &e, const size_t &)
				  { handlePacket(e, dataStruct, callback); });
      }
      else
      {
	std::cout << error.message() << std::endl;
	callback(error);
      }
    };

    template <typename T, typename Callback>
    void handlePacket(const boost::system::error_code& error, T & dataStruct,
		      Callback callback)
    {
      if (!error)
      {
	std::string s(_packetData.data(), _packetData.size());
	Serial::deserializeData(dataStruct, s);
	callback(error);
      }
      else
	callback(error);
    }

    sslSocket 				_socket;
    boost::mutex			_writeMutex;
    std::string 			_dataOut;
    char				_headerBuf[HeaderSize];
    std::vector<char>			_packetData;
};

#endif /* !SPIDER_CONNECTION_HPP */
