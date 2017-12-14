//
// Created by baptiste on 9/25/17.
//

#ifndef SPIDER_CONNECTION_HPP
# define SPIDER_CONNECTION_HPP

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

	sslSocket& getSSLSocket()
	{
		return _socket;
	}

    template <typename Callback>
    void 	handShake(Callback callback) {
      _socket.async_handshake(boost::asio::ssl::stream_base::client, callback);
    }

    template <typename T, typename Callback>
    void write(T & data, Callback callback)
    {
      std::ostringstream            serializedData;
      std::string                 fullData = Serial::serializeData(data);

      if (fullData == "ERROR") {
	callback(boost::asio::error::fault);
	return;
      }
      serializedData << std::setfill('0') << std::setw(HeaderSize) << fullData.size();
	  packet = serializedData.str() + fullData;
      boost::asio::async_write(_socket, boost::asio::buffer(packet), callback);
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
      boost::shared_ptr<std::string> packet = boost::make_shared<std::string>(serializedData.str() + fullData);
      boost::asio::write(_socket, boost::asio::buffer(*packet), e);
      return e;
    }

    template <typename T, typename Callback>
    void read(T & dataStruct, Callback callback)
    {
      void (SSLConnection::*fctPtr)(const boost::system::error_code&, T&, boost::tuple<Callback>)
      		= &SSLConnection::handleHeader<T, Callback>;

	  boost::asio::async_read(_socket, boost::asio::buffer(_headerBuf, 8),
			      boost::bind(fctPtr, this,
					  boost::asio::placeholders::error,
					  boost::ref(dataStruct), boost::make_tuple(callback)));
    }

  private:
    enum : unsigned int{ HeaderSize = 8 };

    template <typename T, typename Callback>
    void handleHeader(const boost::system::error_code& error,
		      T & dataStruct, boost::tuple<Callback> callback)
    {
      unsigned int bytesTransferred = 0;
      std::istringstream iss(_headerBuf);
      void (SSLConnection::*fctPtr)(const boost::system::error_code&, T&, boost::tuple<Callback>)
      = &SSLConnection::handlePacket<T, Callback>;

      if (!error)
      {
	  if (!(iss >> bytesTransferred))
	  {
	    boost::system::error_code e(boost::asio::error::invalid_argument);
	    boost::get<0>(callback)(e);
	    return ;
	  }
	  _packetData.resize(bytesTransferred);
	  boost::asio::async_read(_socket, boost::asio::buffer(_packetData),
				  boost::bind(fctPtr, this,
					      boost::asio::placeholders::error,
					      boost::ref(dataStruct), callback));
      }
      else
		boost::get<0>(callback)(error);
    };

    template <typename T, typename Callback>
    void handlePacket(const boost::system::error_code& error, T & dataStruct,
		      boost::tuple<Callback> callback)
    {
      if (!error)
      {
	std::string s(_packetData.data(), _packetData.size());
	Serial::deserializeData(dataStruct, s);
	boost::get<0>(callback)(error);
      }
      else
	boost::get<0>(callback)(error);
    }

    sslSocket 				_socket;
    std::string 			_dataOut;
    char				_headerBuf[HeaderSize];
    std::vector<char>			_packetData;
	std::string packet;
};

#endif /* !SPIDER_CONNECTION_HPP */
