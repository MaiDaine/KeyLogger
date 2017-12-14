#ifndef LOG_HPP_
 #define LOG_HPP_

#include <vector>
#include "Boost.hpp"

class ImgLog
{
  public:
	ImgLog();
	~ImgLog();

	void	getData(std::string path) const;
	void	setData(const std::string inputFile, const std::string filename);

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
      ar & _data;
      ar & _name;
    }

    std::vector<char>   _data;
    std::string		_name;
};

#endif /* !LOG_HPP_ */

