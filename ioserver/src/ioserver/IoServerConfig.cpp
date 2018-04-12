#include "ioserver/IoServerConfig.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "easyloggingpp/easylogging++.h"
#include "ioserver/IoServerWebUtils.h"

#include "ioserver/IoServerException.h"

#define CONFIGURU_IMPLEMENTATION 1
#include "configuru/configuru.hpp"

namespace r7 {

namespace po = boost::program_options;

const int IoServerConfig::DEFAULT_PORT = 12080;
const std::string IoServerConfig::DEFAULT_CONFIGFILE = "ioserver.conf";
const std::string IoServerConfig::DEFAULT_LOGLEVEL = "DEBUG";
const std::string IoServerConfig::DEFAULT_LOGFILEPATH = "log/ioserver.log";
const std::string IoServerConfig::DEFAULT_DBPATH = "db";


IoServerConfig::IoServerConfig(int argc, char *argv[])
{
    // Declare the supported options.
    po::options_description desc("R7-IoServer command line options. Overrides config file settings.");
    desc.add_options()
            ("help,h", "This message")
            ("port,p", po::value<int>(), "Server port. Defaults: 12080.")
            ("config,c", po::value<std::string>(), "Path to config file. Optional. Default if exists: /etc/ioserver/ioserver.conf or ./ioserver.conf");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // If help option, output and return without continuing
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return;
    }

    // Config file
    std::string configFile = DEFAULT_CONFIGFILE;
    if (vm.count("config")) {
        configFile = vm["config"].as<std::string>();
    }
    if (configFile.empty()) {
        if(boost::filesystem::exists("/etc/ioserver/ioserver.conf")) {
            configFile = "/etc/ioserver/ioserver.conf";
        }
    }
    if (configFile.empty()) {
        DEFAULT_CONFIGFILE;
    }

    // Parse config file if exists
    configuru::Config cfg;
    try {
        if(boost::filesystem::exists(configFile.c_str())) {
            cfg = configuru::parse_file(configFile.c_str(), configuru::JSON);
        }
        else {
            cfg = configuru::parse_string("{}", configuru::JSON, "default");
        }
    }
    catch(std::exception re) {
        throw IoServerException(std::string("Error loading config file. ") + re.what());
    }


    // Port
    try {
        if (vm.count("port")) {
            mPort = vm["port"].as<int>();
        }
        else {
            mPort = cfg.get_or("port", IoServerConfig::DEFAULT_PORT);
        }
    }
    catch(std::exception re) {
        throw IoServerException(std::string("Error parsing port. ") + re.what());
    }

    // Log level
    try {
        if (vm.count("logLevel")) {
            mLogLevel = vm["logLevel"].as<std::string>();
        }
        else {
            mLogLevel = cfg.get_or("logLevel", IoServerConfig::DEFAULT_LOGLEVEL);
        }
    }
    catch(std::exception re) {
        throw IoServerException(std::string("Error parsing log level. ") + re.what());
    }

    // Log File
    try {
        if (vm.count("logFilePath")) {
            mLogFilePath = vm["logFilePath"].as<std::string>();
        }
        else {
            mLogFilePath = cfg.get_or("logFilePath", IoServerConfig::DEFAULT_LOGFILEPATH);
        }
    }
    catch(std::exception re) {
        throw IoServerException(std::string("Error parsing log file path. ") + re.what());
    }

    // Reconfigure logging if needed
    IoServerConfig::configureLogging(mLogLevel, mLogFilePath);

    // Database Path
    try {
        if (vm.count("dbPath")) {
            mDbFilePath = vm["dbPath"].as<std::string>();
        }
        else {
            mDbFilePath = cfg.get_or("dbPath", IoServerConfig::DEFAULT_DBPATH);
        }
    }
    catch(std::exception re) {
        throw IoServerException(std::string("Error parsing db path. ") + re.what());
    }
    try {
        cfg.check_dangling();
    }
    catch(const std::exception& e) {
        LOG(WARNING) << "Dangling keys in config file. " << e.what();
    }

    this->isValid = true;
}

void IoServerConfig::configureLogging(const std::string& logLevelStr, const std::string& logFilePath) {
    //    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    //el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    //defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %loc %msg ");

    if (!logFilePath.empty()) {

        boost::filesystem::path logDir = boost::filesystem::path(logFilePath).parent_path();
        if (!boost::filesystem::is_directory(logDir)) {
            boost::filesystem::create_directories(logDir);
        }

        defaultConf.setGlobally(el::ConfigurationType::Filename, logFilePath.c_str()                                );
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
    }
    else {
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "false");
    }
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level [%logger] %msg");

    el::Level level = el::LevelHelper::convertFromString(logLevelStr.c_str());
    if (level == el::Level::Unknown) {
        LOG(WARNING) << "Unknown log level: " << logLevelStr;
        return;
    }

    // Default easylogging level hiearchy is not valid
    // https://github.com/muflihun/easyloggingpp/issues/634
    switch (level) {
    case el::Level::Trace :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        break;

    case el::Level::Debug :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        break;

    case el::Level::Info :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        break;

    case el::Level::Warning :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        break;

    case el::Level::Error :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        break;

    case el::Level::Fatal :
        defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");

        break;
    }

    el::Loggers::setDefaultConfigurations(defaultConf, true);
    //    el::Loggers::reconfigureAllLoggers(defaultConf);

    //    el::Logger* ioServerLogger = el::Loggers::getLogger("ioserver");
    //    ioServerLogger->info("Starting r7-ioserver 3");

    //    CLOG(INFO, "ioserver") << "Starting r7-ioserver 4b";

    //    el::Logger* defaultLogger = el::Loggers::getLogger("default");
    //    defaultLogger->info("Starting r7-ioserver 1");

    //    LOG(INFO) << "Starting r7-ioserver 2";

    //    el::Logger* ioServerLogger = el::Loggers::getLogger("ioserver");
    //    ioServerLogger->info("Starting r7-ioserver 3");

    //    CLOG(INFO, "ioserver") << "Starting r7-ioserver 4b";

}


IoServerConfig::~IoServerConfig()
{
    //dtor
}

}
