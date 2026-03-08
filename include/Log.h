#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

class Log {
public:
    // Función estática para imprimir mensajes
    static void print(const std::string& text) {
        std::cout << text << "\n";
    }
};

#endif // LOG_H#pragma once
