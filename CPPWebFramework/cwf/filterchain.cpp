/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "filterchain.h"
#include "configuration.h"
#include "constants.h"

CWF_BEGIN_NAMESPACE

FilterChain::FilterChain(HttpServlet *servlet, const Configuration &configuration) :  servlet(servlet), configuration(configuration)
{
}

void FilterChain::doFilter(CWF::HttpServletRequest &request, CWF::HttpServletResponse &response)
{
    if(servlet != nullptr)
    {
        const HttpParser &parser = request.getHttpParser();
        if(parser.getMethod()      == HTTP::METHOD::GET)
            servlet->doGet(request, response);
        else if(parser.getMethod() == HTTP::METHOD::POST)
            servlet->doPost(request, response);
        else if(parser.getMethod() == HTTP::METHOD::PUT)
            servlet->doPut(request, response);
        else if(parser.getMethod() == HTTP::METHOD::DELETE)
            servlet->doDelete(request, response);
        else if(parser.getMethod() == HTTP::METHOD::OPTIONS)
            servlet->doOptions(request, response);
        else if(parser.getMethod() == HTTP::METHOD::TRACE)
            servlet->doTrace(request, response);
    }
    else
    {
        FileManager fileManager;
        QString url  = request.getRequestURL();
        QString path = request.getPath();
        const QString &extention = fileManager.fileExtention(url);

        if(url == FILE_EXTENTION::BAR)
        {
            request.getRequestDispatcher(configuration.getIndexPage()).forward(request, response);
        }
        else if(extention == FILE_EXTENTION::HTML || extention == FILE_EXTENTION::HTM)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::TEXT_HTML_UTF8);
        }
        else if(extention == FILE_EXTENTION::CSS || extention == FILE_EXTENTION::TXT || extention == FILE_EXTENTION::PHP)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, ("text/" + extention.toLatin1() + "; charset=UTF-8") );
        }
        else if(extention == FILE_EXTENTION::ICO)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::IMAGE_MICROSOFT_ICO);
        }
        else if(extention == FILE_EXTENTION::PNG || extention == FILE_EXTENTION::GIF || extention == FILE_EXTENTION::BMP)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, ("image/" + extention.toLatin1()));
        }
        else if(extention == FILE_EXTENTION::JPE || extention == FILE_EXTENTION::JPG)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::IMAGE_JPEG);
        }
        else if(extention == FILE_EXTENTION::TIFF || extention == FILE_EXTENTION::TIF)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::IMAGE_TIFF);
        }
        else if(extention == FILE_EXTENTION::SVG || extention == FILE_EXTENTION::SVGZ)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::IMAGE_SVG_XML);
        }
        else if(extention == FILE_EXTENTION::PDF  || extention == FILE_EXTENTION::XML ||
                extention == FILE_EXTENTION::JSON || extention == FILE_EXTENTION::ZIP)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, ("application/" + extention.toLatin1()) );
        }
        else if(extention == FILE_EXTENTION::MP3)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::AUDIO_MP3);
        }
        else if(extention == FILE_EXTENTION::MP4)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::AUDIO_MP4);
        }
        else if(extention == FILE_EXTENTION::FLV)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::VIDEO_FLV);
        }
        else if(extention == FILE_EXTENTION::DOC)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_MSWORD);
        }
        else if(extention == FILE_EXTENTION::RTF)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_RTF);
        }
        else if(extention == FILE_EXTENTION::XLS)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_EXCEL);
        }
        else if(extention == FILE_EXTENTION::PPT)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_POWER_POINT);
        }
        else if(extention == FILE_EXTENTION::JS)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_JAVASCRIPT);
        }
        else if(extention == FILE_EXTENTION::ODT)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_OPEN_DOCUMENT_TEXT);
        }
        else if(extention == FILE_EXTENTION::ODS)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_OPEN_DOCUMENT_SPREADSHEET);
        }
        else if(extention == FILE_EXTENTION::SWF)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_SHOCKWAVE_FLASH);
        }
        else if(extention == FILE_EXTENTION::RAR)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_RAR_COMPRESSED);
        }
        else if(extention == FILE_EXTENTION::EXE || extention == FILE_EXTENTION::MSI)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_MS_DOWNLOAD);
        }
        else if(extention == FILE_EXTENTION::CAB)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_CAB_COMPRESSED);
        }
        else if(extention == FILE_EXTENTION::PSD)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_PHOTOSHOP);
        }
        else if(extention == FILE_EXTENTION::AI || extention == FILE_EXTENTION::EPS ||
                extention == FILE_EXTENTION::PS)
        {
            write(response, path, url, HTTP::CONTENT_TYPE, HTTP::APPLICATION_POSTSCRIPT);
        }
        else if(extention == FILE_EXTENTION::INI)
        {
            QString file(std::move(fileManager.fileName(url)));
            if(file != CONFIGURATION::CPP_WEB_INI)
                write(response, path, url, HTTP::CONTENT_TYPE, ("text/" + extention.toLatin1() + "; charset=UTF-8") );
            else if(configuration.getAccessServerPages())
                write(response, path, url, HTTP::CONTENT_TYPE, ("text/" + extention.toLatin1() + "; charset=UTF-8") );
            else
                request.getRequestDispatcher(STATUS::STATUS_401).forward(request, response);
        }
        else
        {
            response.setStatus(HttpServletResponse::SC_NOT_FOUND, STATUS::NOT_FOUND);
            response.addHeader("Content-Type; charset=UTF-8", "text/html");
            request.getRequestDispatcher(STATUS::STATUS_404).forward(request, response);
        }
    }
}

void FilterChain::write(HttpServletResponse &response, const QString &path, const QString &url, const QByteArray &name, const QByteArray &value) const
{
    QFile file(path + url);
    if(file.open(QIODevice::ReadOnly))
    {
        response.addHeader(name, value);
        response.write(file.readAll());
    }
    else
    {
        response.sendError(0, file.errorString().toLatin1());
    }
}

CWF_END_NAMESPACE
