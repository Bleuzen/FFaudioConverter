/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018-2024  Bleuzen
https://github.com/Bleuzen/FFaudioConverter
supgesu@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------- */

#ifndef UTIL_H
#define UTIL_H

#include <QString>

class Util
{
public:
    static bool isBinary(QString file);
    static bool prepareAndCheckDirectory(QString dir);
    static bool isNullOrEmpty(QString string);
    static bool mayBeAudioOrVideoFile(QString file);
};

#endif // UTIL_H
