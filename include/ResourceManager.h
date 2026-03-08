#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <raylib.h>
#include <iostream>
#include "Log.h"

// -----------------------------
// ResourceManager: carga y gestiona recursos de Raylib
// -----------------------------
class ResourceManager {
public:
    // -----------------------------
    // Acceso a la instancia (Singleton)
    // -----------------------------
    static ResourceManager& instance() {
        static ResourceManager rm;
        return rm;
    }

    // -----------------------------
    // Obtener textura (carga automática si no existe)
    // -----------------------------
    std::shared_ptr<Texture2D> getTexture(const std::string& file) {
        std::string fullPath = texturePath + file;          // Concatenar path base
        auto it = textures.find(fullPath);
        if (it != textures.end()) return it->second;       // Si ya está cargada, devolver

        // Verificar si existe archivo
        if (!FileExists(fullPath.c_str())) {
            Log::print("ERROR: No se encontró textura: " + fullPath);
            return nullptr;
        }

        auto tex = std::make_shared<Texture2D>(LoadTexture(fullPath.c_str())); // Cargar
        textures[fullPath] = tex;    // Guardar
        return tex;
    }

    // -----------------------------
    // Obtener sonido (carga automática si no existe)
    // -----------------------------
    std::shared_ptr<Sound> getSound(const std::string& file) {
        std::string fullPath = soundPath + file;
        auto it = sounds.find(fullPath);
        if (it != sounds.end()) return it->second;

        if (!FileExists(fullPath.c_str())) {
            Log::print("ERROR: No se encontró sonido: " + fullPath);
            return nullptr;
        }

        auto s = std::make_shared<Sound>(LoadSound(fullPath.c_str()));
        sounds[fullPath] = s;
        return s;
    }


    // -----------------------------
// Obtener sonido (carga automática si no existe)
// -----------------------------
    std::shared_ptr<Music> getMusic(const std::string& file) {
        std::string fullPath = musicPath + file;
        auto it = music.find(fullPath);
        if (it != music.end()) return it->second;

        if (!FileExists(fullPath.c_str())) {
            Log::print("ERROR: No se encontró la música: " + fullPath);
            return nullptr;
        }

        auto m = std::make_shared<Music>(LoadMusicStream(fullPath.c_str()));
        music[fullPath] = m;
        return m;
    }

    // -----------------------------
    // Obtener fuente (carga automática si no existe)
    // -----------------------------
    std::shared_ptr<Font> getFont(const std::string& file) {
        std::string fullPath = fontPath + file;
        auto it = fonts.find(fullPath);
        if (it != fonts.end()) return it->second;

        if (!FileExists(fullPath.c_str())) {
            Log::print("ERROR: No se encontró fuente: " + fullPath);
            return nullptr;
        }

        auto f = std::make_shared<Font>(LoadFontEx(fullPath.c_str(),32,0,250));
        fonts[fullPath] = f;
        return f;
    }

    // -----------------------------
    // Limpiar todos los recursos
    // -----------------------------
    void clear() {
        for (auto& t : textures) UnloadTexture(*t.second);
        for (auto& s : sounds)   UnloadSound(*s.second);
        for (auto& f : fonts)    UnloadFont(*f.second);
		for (auto& m : music)    UnloadMusicStream(*m.second);
        
        textures.clear();
        sounds.clear();
        fonts.clear();
		music.clear();
    }

private:
    // Constructor y copia privada (Singleton)
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // -----------------------------
    // Mapas internos de recursos
    // -----------------------------
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> sounds;
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
	std::unordered_map<std::string, std::shared_ptr<Music>> music;

    // Paths base
    std::string texturePath = "assets/textures/";
    std::string soundPath = "assets/sounds/";
	std::string musicPath = "assets/music/";
    std::string fontPath = "assets/fonts/";
};
