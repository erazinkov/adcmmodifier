#include "mapperparser.h"

MapperParser::MapperParser(QCommandLineParser &parser, MapperQuery &query) : parser_(parser), query_(query)
{
}

bool MapperParser::parseResult()
{
    using Status = CommandLineParseResult::Status;

    auto r{false};

    auto parseCommandLineResult{parseCommandLine()};
    switch (parseCommandLineResult.statusCode) {
        case Status::Ok:
        {
            r = true;
            break;
        }
        case Status::Error:
        {
            std::fputs(qPrintable(parseCommandLineResult.errorString.value_or("Unknown error occurred")),
                       stderr);
            std::fputs("\n\n", stderr);
            std::fputs(qPrintable(parser_.helpText()), stderr);
            break;
        }
        case Status::VersionRequested:
            parser_.showVersion();
        case Status::HelpRequested:
            parser_.showHelp();
    }

    return r;
}

MapperParser::CommandLineParseResult MapperParser::parseCommandLine()
{
    using Status = CommandLineParseResult::Status;

    parser_.setApplicationDescription("Adcm data accumulation program.");
    parser_.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    const QCommandLineOption aOption("a", "any (unused)", "a");

    parser_.addOption(aOption);

    parser_.addPositionalArgument("input", "Path to input data file ('adcm.dat').");
    parser_.addPositionalArgument("output", "Path to output data file.");
    parser_.addPositionalArgument("time", "Time to accumulate data in min.");
    const QCommandLineOption helpOption = parser_.addHelpOption();
    const QCommandLineOption versionOption = parser_.addVersionOption();

    if (!parser_.parse(QCoreApplication::arguments()))
        return { Status::Error, parser_.errorText() };

    if (parser_.isSet(versionOption))
        return { Status::VersionRequested };

    if (parser_.isSet(helpOption))
        return { Status::HelpRequested };

//    if (parser_.isSet(aOption)) {
//        bool ok;
//        query_.a = parser_.value(aOption).toUInt(&ok);
//        if (query_.a < 0 || !ok)
//        {
//            return { Status::Error, QString("Incorrect a number: %1").arg(query_.a) };
//        }
//    }

    const QStringList positionalArguments = parser_.positionalArguments();
    if (positionalArguments.isEmpty() || positionalArguments.size() < 3 || positionalArguments.size() > 3)
    {
        return { Status::Error, "Three arguments are required." };
    }
    query_.input = positionalArguments.at(0);
    query_.output = positionalArguments.at(1);
    bool ok;
    query_.time = positionalArguments.at(2).toInt(&ok);
    if (query_.time < 0 || !ok)
    {
        return { Status::Error, QString("Incorrect 'time' parameter: %1").arg(query_.a) };
    }
    return { Status::Ok };
}
