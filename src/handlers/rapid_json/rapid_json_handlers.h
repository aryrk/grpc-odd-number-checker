#ifndef RAPIDJSONHANDLERS_H
#define RAPIDJSONHANDLERS_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

template<typename T>
void WriteRapidJSON(rapidjson::Document &document, std::string_view key, T value);

std::string ReadRapidJSON(rapidjson::Document &document);

#endif