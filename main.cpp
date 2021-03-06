#include <QCoreApplication>
#include <QDomDocument>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <iostream>

#include "protocolparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int Return = 1;
    bool nodoxygen = false;
    bool nomarkdown = false;
    bool nohelperfiles = false;

    // The list of arguments
    QStringList arguments = a.arguments();

    if(arguments.size() <= 1)
    {
        std::cout << "Protocol generator usage:" << std::endl;
        std::cout << "ProtoGen input.xml [outputpath] [-no-doxygen] [-no-markdown] [-no-helper-files]" << std::endl;
        return 0;
    }


    // We expect the input file here
    QString filename = a.arguments().at(1);

    // The output path
    QString path;

    // Skip the first argument "ProtoGen.exe"
    for(int i = 1; i < arguments.size(); i++)
    {
        QString arg = arguments.at(i);
        if(arg.contains("-no-doxygen", Qt::CaseInsensitive))
            nodoxygen = true;
        else if(arg.contains("-no-markdown", Qt::CaseInsensitive))
            nomarkdown = true;        
        else if(arg.contains("-no-helper-files", Qt::CaseInsensitive))
            nohelperfiles = true;
        else if(arg.endsWith(".xml"))
            filename = arg;
        else if(arg != filename)
            path = arg;
    }

    if(!filename.isEmpty())
    {
        QDomDocument doc("protogen");

        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            if (doc.setContent(&file))
            {
                ProtocolParser parser;

                // Set our working directory
                if(!path.isEmpty())
                {
                    QDir dir(QDir::current());

                    // The path could be absolute or relative to
                    // our current path, this works either way

                    // Make sure the path exists
                    dir.mkpath(path);

                    // Now set it as the path to use
                    QDir::setCurrent(path);
                }

                if(parser.parse(doc, nodoxygen, nomarkdown, nohelperfiles))
                    Return = 1;

            }
            else
            {
                std::cout << "failed to validate xml from file: " << filename.toStdString() << std::endl;
                Return = 0;
            }

            file.close();
        }
        else
        {
            std::cout << "failed to open protocol file: " << filename.toStdString() << std::endl;
            Return = 0;
        }

    }
    else
    {
        std::cout << "must provide a protocol file." << std::endl;
        Return = 0;
    }

    return Return;
}
