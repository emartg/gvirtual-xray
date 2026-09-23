#ifndef GVXR_LOGGER_H
#define GVXR_LOGGER_H


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <fstream>
#include <ostream>
#include <streambuf>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class Logger;

class LoggerStreamBuffer : public std::streambuf
{
public:
    void setTargets(std::streambuf* aFileBuffer, std::streambuf* aConsoleBuffer);

protected:
    int overflow(int aCharacter = traits_type::eof()) override;
    std::streamsize xsputn(const char* apText, std::streamsize aCount) override;
    int sync() override;

private:
    std::streambuf* m_file_buffer = nullptr;
    std::streambuf* m_console_buffer = nullptr;
};

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

    LoggerStreamBuffer m_stream_buffer;
    std::ostream m_stream;

    int m_last_progress_as_int;
};


}


#endif // GVXR_LOGGER_H
