#ifndef GVXR_LOGGER_H
#define GVXR_LOGGER_H


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <fstream>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class Logger;

extern Logger LOGGER;

class Logger
{
public:
    Logger(const char* aFileName = "gvxr.log");
    Logger(const std::string& aFileName = "gvxr.log");
    ~Logger();

    bool is_open() const;
    void open(const std::string& aFileName);
    void close();
    void flush();

    const std::string& getFilename() const;
    const std::string& getFname() const;

    std::ostream& logNow(const std::string& aMessage = "");
    std::ostream& logError(const std::string& aMessage = "");
    std::ostream& logWarning(const std::string& aMessage = "");

    void progressBar(float aPercentage, bool aNewLineFlag = false);

    void progressBar(unsigned int aProgress, unsigned int aTotal, bool aNewLineFlag = false);

protected:
    std::string getNow() const;

    Logger& operator=(const Logger& aLogger);
    Logger(const Logger& aLogger);

    std::ofstream m_output;
    std::string m_filename;

    int m_last_progress_as_int;
};


}


#endif // GVXR_LOGGER_H
