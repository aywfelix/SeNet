#include "Assertx.h"
#include "JsonConfig.h"
#include "LogUtil.h"

std::unique_ptr<LogUtil> g_pLog = nullptr;

#define RED	"\x1b[31m"
#define YELLOW	"\x1b[33m"
#define GREEN	"\x1b[32m"
#define WHITE	"\x1b[37m"
#define END	"\x1b[0m"

bool LogUtil::Init(std::string servername)
{
	m_servername = servername;
	return true;
}

bool LogUtil::CreateLog()
{
	int64_t now = time(0);
	m_logname = m_servername + "_" + std::to_string(now / 60) + ".log"; // 取分钟数日志的名称为：服务器名_分钟数.log
	return true;
}

void LogUtil::ThreadLoop()
{
	for (;;)
	{
		if (m_stop) break;
		CreateLog();
		SendLog();
		sf_sleep(50);
	}
}

bool LogUtil::SendLog()
{
	std::string info = "";
	if (m_queue.TryPopQ(info))
	{
#ifdef DEBUG
		std::cout << info << std::endl; 
#endif
	}
	return true;
}

void LogUtil::Start()
{
	m_stop = false;
	m_thread = std::thread(std::bind(&LogUtil::ThreadLoop, this));
}

void LogUtil::Stop()
{
	m_stop = true;
	if (m_thread.joinable()) m_thread.join();
}

void LogUtil::Log(int level, const char* func, int line, const char* fmt, ...)
{
	if (level < m_level) return;

	UniqueLock lock(m_mutex);
	m_oss.clear();
	m_oss.str("");

	char content[1024];
	memset(content, '\0', sizeof(content));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(content, sizeof(content) - 1, fmt, ap);
	va_end(ap);

#ifdef DEBUG
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		m_oss << RED << "[fatal]" << func << ":" << line << ">>" << content << END;  //red
	}
	else if (level == E_LOG_WARN)
	{
		m_oss << YELLOW << "[warn]" << func << ":" << line << ">>" << content << END; //yellow
	}
	else if (level == E_LOG_INFO)
	{
		m_oss << GREEN << "[info]" << func << ":" << line << ">>" << content << END; //green
	}
	else if (level == E_LOG_DEBUG)
	{
		m_oss << WHITE << "[debug]" << func << ":" << line << ">>" << content << END; //white
	}
#else
	m_oss << line << "|" << func << "|" << content;
#endif
	m_oss << "\n";
	m_queue.PutQ(m_oss.str());
}

bool LogUtil::LoadLogCfg(std::string& logcfg)
{
	return LoadLogCfg(logcfg.c_str());
}

bool LogUtil::LoadLogCfg(const char* logcfg)
{
	JsonConfig json_config;
	if (!json_config.Load(logcfg))
	{
		AssertEx(0, "log path cfg error");
		return false;
	}
	m_level = json_config.m_Root["Log"]["level"].asInt();
	m_roll_type = json_config.m_Root["Log"]["roll"].asInt();
	m_LogPath = json_config.m_Root["Log"]["path"].asString();
	m_roll_size = json_config.m_Root["Log"]["roll_size"].asInt64();
	return true;
}

LogStream& LogUtil::Stream(int level, const char* func, int line)
{
	if (m_level < g_pLog->GetLevel()) { return m_logstream; }

	m_logstream.Init(level, func, line);
	return m_logstream;
}

void LogStream::Init(int level, const char* func, int line)
{
	m_oss.clear();
	m_oss.str("");
	m_level = level;
	m_func = func;
	m_line = line;

#ifdef DEBUG
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		m_oss << RED << "[fatal]" << func << ":" << line << ">>" << content;  //red
	}
	else if (level == E_LOG_WARN)
	{
		m_oss << YELLOW << "[warn]" << func << ":" << line << ">>" << content; //yellow
	}
	else if (level == E_LOG_INFO)
	{
		m_oss << GREEN << "[info]" << func << ":" << line << ">>" << content; //green
	}
	else if (level == E_LOG_DEBUG)
	{
		m_oss << WHITE << "[debug]" << func << ":" << line << ">>" << content; //white
	}
#else
	m_oss << func << ":" << line << ">>";
#endif
}

void LogStream::Clear()
{
#ifdef DEBUG
	m_oss << VOID;
#endif // DEBUG
	m_oss << "\n";
	g_pLog->GetQueue().PutQ(m_oss.str());
}

void LogUtil::LogGame(int type, const char* fmt, ...)
{
	m_oss.clear();
	m_oss.str("");

	char content[256];
	memset(content, '\0', sizeof(content));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(content, sizeof(content) - 1, fmt, ap);
	va_end(ap);

	m_oss << content << "\n";
	// 处理业务日志信息
	std::cout << m_oss.str() << std::endl;
}