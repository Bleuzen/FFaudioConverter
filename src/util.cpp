#include "util.h"

bool Util::isBinary(QString file) {
    if (file.length() <= 0) return false;
    QFileInfo fileInfo(file);
    return (fileInfo.exists() && fileInfo.isFile() && fileInfo.isExecutable());
}
