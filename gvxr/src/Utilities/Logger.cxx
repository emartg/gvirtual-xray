

//******************************************************************************
//  Include
//******************************************************************************
#include <chrono>
#include <ctime>
#include <sstream>
#include<iostream>
#include<cstring>
#include<cmath>
#include <algorithm>

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//  Global variable
//******************************************************************************
gVirtualXRay::Logger gVirtualXRay::LOGGER("");


//******************************************************************************
//  namespace
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------------------------------------
void LoggerStreamBuffer::setTargets(std::streambuf* aFileBuffer,
                                    std::streambuf* aConsoleBuffer)
//-------------------------------------------------------------
{
    m_file_buffer = aFileBuffer;
    m_console_buffer = aConsoleBuffer;
}


//--------------------------------------------------
int LoggerStreamBuffer::overflow(int aCharacter)
//--------------------------------------------------
{
    if (aCharacter != traits_type::eof())
    {
        const char character = static_cast<char>(aCharacter);
        if (m_file_buffer) m_file_buffer->sputc(character);
        if (m_console_buffer) m_console_buffer->sputc(character);
    }

    return traits_type::not_eof(aCharacter);
}


//--------------------------------------------------------------------------
std::streamsize LoggerStreamBuffer::xsputn(const char* apText,
                                           std::streamsize aCount)
//--------------------------------------------------------------------------
{
    if (m_file_buffer) m_file_buffer->sputn(apText, aCount);
    if (m_console_buffer) m_console_buffer->sputn(apText, aCount);
    return aCount;
}


//----------------------------
int LoggerStreamBuffer::sync()
//----------------------------
{
    if (m_file_buffer) m_file_buffer->pubsync();
    if (m_console_buffer) m_console_buffer->pubsync();
    return 0;
}


//------------------------------------
Logger::Logger(const char* aFileName):
//------------------------------------
    m_stream(&m_stream_buffer),
    m_last_progress_as_int(-1)
//------------------------------------
{
    open(aFileName);
}


//-------------------------------------------
Logger::Logger(const std::string& aFileName):
//-------------------------------------------
    m_stream(&m_stream_buffer),
    m_last_progress_as_int(-1)
//-------------------------------------------
{
    open(aFileName);
}


//---------------
Logger::~Logger()
//---------------
{
    close();
}


//--------------------------
bool Logger::is_open() const
//--------------------------
{
    return m_output.is_open();
}


//---------------------------------------------
void Logger::open(const std::string& aFileName)
//---------------------------------------------
{
    // The filename has changed, close the previous log file if any
    if (m_filename != aFileName)
    {
        close();
    }

    // Update the filename
    m_filename = aFileName;

    // Open the stream if needed
    if (m_filename.size())
    {
        m_output.open (m_filename, std::ofstream::out | std::ofstream::app);

        // It is not open
        if (!is_open())
        {
            std::clog << "Warning, cannot open the log file \"" << m_filename << "\"." << std::endl;
        }
        // It is open
        else
        {
            m_stream_buffer.setTargets(m_output.rdbuf(), std::clog.rdbuf());
            m_output << std::endl;
            m_output << std::endl;
            m_output << "--------------------------------------------------------------------------------" << std::endl;
            logNow("Open log file") << "\t" << m_filename << std::endl;
            m_output << "--------------------------------------------------------------------------------" << std::endl;
        }
    }
}


//------------------
void Logger::close()
//------------------
{
    if (is_open())
    {
        m_output << "--------------------------------------------------------------------------------" << std::endl;
        logNow("Close log file") << "\t" << m_filename << std::endl;
        m_output << "--------------------------------------------------------------------------------" << std::endl;
        m_output.close();
        m_filename.clear();
        m_stream_buffer.setTargets(nullptr, nullptr);
    }
}


//------------------
void Logger::flush()
//------------------
{
    if (is_open())
    {
        m_output << std::flush;
    }
    else
    {
        std::cout << std::flush;
        std::cerr << std::flush;
        std::clog << std::flush;
    }
}


//--------------------------------------------
const std::string& Logger::getFilename() const
//--------------------------------------------
{
    return m_filename;
}


//-----------------------------------------
const std::string& Logger::getFname() const
//-----------------------------------------
{
    return m_filename;
}


//----------------------------------------------
std::ostream& Logger::logNow(const std::string& aMessage)
//----------------------------------------------
{
    if (is_open())
    {
        m_stream << getNow() << " ---- " << aMessage;
        return m_stream;
    }
    else
    {
        flush();

        std::clog << getNow() << " ---- " << aMessage;
        return std::clog;
    }
}


//------------------------------------------------
std::ostream& Logger::logError(const std::string& aMessage)
//------------------------------------------------
{
    if (is_open())
    {
        m_stream << getNow() << " (EE) " << aMessage;
        return m_stream;
    }
    else
    {
        flush();

        std::cerr << getNow() << " (EE) " << aMessage;
        return std::cerr;
    }
}


//--------------------------------------------------
std::ostream& Logger::logWarning(const std::string& aMessage)
//--------------------------------------------------
{
    if (is_open())
    {
        m_stream << getNow() << " (WW) " << aMessage;
        return m_stream;
    }
    else
    {
        flush();

        std::cerr << getNow() << " (WW) " << aMessage;
        return std::cerr;
    }
}


//------------------------------------------------------------
void Logger::progressBar(float aPercentage, bool aNewLineFlag)
//------------------------------------------------------------
{
    const int total_length = 80;
    const int number_of_Xs = std::clamp((int) round(std::clamp(aPercentage, 0.0f, 1.0f) * total_length), 0, total_length);
    const int percentage_as_int = round(100.0 * aPercentage);

    // Only update if needed
    if (percentage_as_int != m_last_progress_as_int)
    {
        if (is_open())
        {
            m_output <<
                "\r[" <<
                std::string(number_of_Xs, 'X') <<
                std::string(total_length - number_of_Xs, '-') <<
                "] " <<
                percentage_as_int << "%" <<
                std::flush;

            if (aNewLineFlag) m_output << std::endl;
        }
        else
        {
            std::clog << 
                "\r[" <<
                std::string(number_of_Xs, 'X') <<
                std::string(total_length - number_of_Xs, '-') <<
                "] " <<
                percentage_as_int << "%" <<
                std::flush;

            if (aNewLineFlag) std::clog << std::endl;
        }
    }

    // Backup the current value
    m_last_progress_as_int = percentage_as_int;

    // Reset
    if (percentage_as_int >= 100)
    {
        m_last_progress_as_int = -1;
    }
}


//--------------------------------------------------------------------------------------
void Logger::progressBar(unsigned int aProgress, unsigned int aTotal, bool aNewLineFlag)
//--------------------------------------------------------------------------------------
{
    float percentage = float(aProgress) / float(aTotal);

    progressBar(percentage, aNewLineFlag);
}


//--------------------------------
std::string Logger::getNow() const
//--------------------------------
{
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::string message = std::ctime(&current_time);
    message.pop_back(); // Drop std::endl
    return message;
}


//----------------------------------------------
Logger& Logger::operator=(const Logger& aLogger)
//----------------------------------------------
{
    // Do nothing
    return *this;
}


//-----------------------------------
Logger::Logger(const Logger& aLogger)
//-----------------------------------
:
    m_stream(&m_stream_buffer)
{
    // Do nothing
}
