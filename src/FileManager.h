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
            Serial.print("Ошибка открытия файла для записи: ");
            Serial.println(filename);
            return false;
        }
        file.print(data);
        file.close();
        Serial.print("Записано в файл: ");
        Serial.println(filename);
        Serial.println(" данные: ");
        Serial.println(data);
        return true;
    }

    static String loadFromFile(const char *filename)
    {
        File file = LittleFS.open(filename, "r");
        if (!file)
        {
            Serial.println("Ошибка открытия файла для чтения: ");
            Serial.print(filename);
            return "";
        }
        String data = file.readString();
        file.close();
        Serial.print("Прочтено из файла: ");
        Serial.println(filename);
        Serial.println(" данные: ");
        Serial.println(data);
        return data;
    }
};

#endif
