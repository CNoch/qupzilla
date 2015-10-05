/* ============================================================
* QupZilla - QtWebEngine based browser
* Copyright (C) 2015 David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

#include "networkurlinterceptor.h"
#include "urlinterceptor.h"
#include "settings.h"

NetworkUrlInterceptor::NetworkUrlInterceptor(QObject *parent)
    : QWebEngineUrlRequestInterceptor(parent)
    , m_sendDNT(false)
{
}

bool NetworkUrlInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    bool result = false;

    if (m_sendDNT) {
        result = true;
        info.setExtraHeader(QByteArrayLiteral("DNT"), QByteArrayLiteral("1"));
    }

    foreach (UrlInterceptor *interceptor, m_interceptors) {
        if (interceptor->interceptRequest(info))
            result = true;
    }

    return result;
}

void NetworkUrlInterceptor::installUrlInterceptor(UrlInterceptor *interceptor)
{
    if (!m_interceptors.contains(interceptor))
        m_interceptors.append(interceptor);
}

void NetworkUrlInterceptor::removeUrlInterceptor(UrlInterceptor *interceptor)
{
    m_interceptors.removeOne(interceptor);
}

void NetworkUrlInterceptor::loadSettings()
{
    Settings settings;
    settings.beginGroup("Web-Browser-Settings");
    m_sendDNT = settings.value("DoNotTrack", false).toBool();
    settings.endGroup();
}
