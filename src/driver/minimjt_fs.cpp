#include "driver/minimjt_fs.h"

MiniMjtFs::MiniMjtFs() {}

MiniMjtFs::~MiniMjtFs() {}

String MiniMjtFs::ListDir(const char *dirName, int8_t levels)
{
    if (m_fs == NULL) {
        Serial.println(F("FS not Mounted"));
        return "";
    }
    String result;
    result = "Listing directory: " + String(dirName) + "\n";

    File root = m_fs->open(dirName);
    if (!root) {
        Serial.println(F("- failed to open directory"));
        return "";
    }
    if (!root.isDirectory()) {
        Serial.println(F("- not a directory"));
        return "";
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            result += " DIR: " + String(file.name()) + "\n";
            if (levels > 0) {
                result += ListDir(file.path(), levels - 1);
            }
            if (levels == -1) {
                result += ListDir(file.path(), -1);
            }
        } else {
            result += " FILE: " + String(file.name()) + " | SIZE: " + file.size() + "\n";
        }
        file = root.openNextFile();
    }
    return result;
}

int16_t MiniMjtFs::ReadFile(const char *path, uint8_t *info)
{
    FS_IS_NULL(m_fs)
    Serial.printf("Reading file: %s\r\n", path);

    File file = m_fs->open(path);
    uint16_t retLen = -1;
    if (!file || file.isDirectory()) {
        Serial.println(F("- failed to open file for reading"));
        return retLen;
    }
    retLen = 0;
    while (file.available()) {
        retLen += file.read(info + retLen, 15);
    }
    file.close();
    return retLen;
}

int8_t MiniMjtFs::WriteFile(const char *path, const char *message)
{
    FS_IS_NULL(m_fs)
    int8_t ret = -1;
    Serial.printf("Writing file: %s\r\n", path);

    File file = m_fs->open(path, FILE_WRITE);
    if (!file) {
        Serial.println(F("- failed to open file for writing"));
        return ret;
    }
    if (file.print(message)) {
        Serial.println(F("- file written"));
        ret = 0;
    } else {
        Serial.println(F("- write failed"));
    }
    file.close();
    return ret;
}

int8_t MiniMjtFs::AppendFile(const char *path, const char *message)
{
    FS_IS_NULL(m_fs)
    int ret = -1;
    Serial.printf("Appending to file: %s\r\n", path);

    File file = m_fs->open(path, FILE_APPEND);
    if (!file) {
        Serial.println(F("- failed to open file for appending"));
        return ret;
    }
    if (file.print(message)) {
        Serial.println(F("- message appended"));
        ret = 0;
    } else {
        Serial.println(F("- append failed"));
    }
    file.close();
    return ret;
}

int8_t MiniMjtFs::RenameFile(const char *path1, const char *path2)
{
    FS_IS_NULL(m_fs)
    int8_t ret = 0;
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (m_fs->rename(path1, path2)) {
        Serial.println(F("- file renamed"));
    } else {
        Serial.println(F("- rename failed"));
        ret = -1;
    }
    return ret;
}

int8_t MiniMjtFs::DeleteFile(const char *path)
{
    FS_IS_NULL(m_fs)
    int8_t ret = 0;
    Serial.printf("Deleting file: %s\r\n", path);
    if (m_fs->remove(path)) {
        Serial.println(F("- file deleted"));
    } else {
        Serial.println(F("- delete failed"));
        ret = -1;
    }
    return ret;
}

bool MiniMjtFs::FileExists(const char *path)
{
    FS_IS_NULL(m_fs)
    return m_fs->exists(path);
}
