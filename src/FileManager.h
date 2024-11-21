#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <LittleFS.h>

class FileManager
{
public:
    static bool begin()

    {
        return LittleFS.begin();
    }

    static bool saveToFile(const char *filename, const String &data)
    {
        File file = LittleFS.open(filename, "w");
        if (!file)
        {
            Serial.println("Ошибка открытия файла для записи: " + String(filename));
            return false;
        }
        file.print(data);
        file.close();
        Serial.println(String("Записано в файл: ") + filename + " данные: " + data.c_str());
        return true;
    }

    static String loadFromFile(const char *filename)
    {
        File file = LittleFS.open(filename, "r");
        if (!file)
        {
            Serial.println("Ошибка открытия файла для чтения: " + String(filename));
            return "";
        }
        String data = file.readString();
        file.close();
        Serial.println(String("Прочтено из файла: ") + filename + " данные: " + data);
        return data;
    }
};

#endif // FILE_MANAGER_H
