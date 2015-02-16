//
//  HeaderParser.cpp
//  Proxy
//
//  Created by Seán Hargadon on 16/02/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#include "HeaderParser.h"


HeaderParser::HeaderParser(char* buffer, int buffer_size)
{
    this->buffer = buffer;
    this->current_char = 0;
    this->buffer_size = buffer_size;
    this->current_name_value = 0;
}


char HeaderParser::getCurrentChar()
{
    char c = 0;
    
    if ( this->current_char >= 0 && this->current_char < this->buffer_size ) {
        c = this->buffer[this->current_char];
    }
    else if ( this->current_char >= this->buffer_size ) {
        return 0;
    }
    return c;
}


std::vector<std::pair<std::string, std::string>> HeaderParser::parseHeader()
{
    parseStatus();
    parseNameValueList();
    if (getCurrentChar() == '\r') {
        this->current_char++;
    }
    else {
        
    }
    if (getCurrentChar() == '\n') {
        this->current_char++;
    }
    else {
        
    }
    return this->name_values;
}

void HeaderParser::parseStatus()
{
    parseName();
    if (getCurrentChar() == ' ') {
        this->current_char++;
    }
    parseValue();
    if (getCurrentChar() == '\r') {
        this->current_char++;
    }
    else {
        
    }
    if (getCurrentChar() == '\n') {
        this->current_char++;
    }
    else {
        
    }
}

void HeaderParser::parseNameValue()
{
    parseName();
    if (getCurrentChar() == ':') {
        this->current_char++;
    }
    else {
        
    }
    if (getCurrentChar() == ' ') {
        this->current_char++;
    }
    else {
        
    }
    parseValue();
    if (getCurrentChar() == '\r') {
        this->current_char++;
    }
    else {
        
    }
    if (getCurrentChar() == '\n') {
        this->current_char++;
    }
    else {
        
    }
}

void HeaderParser::parseNameValueList()
{
    if ( !isalpha( getCurrentChar() ) ) {
        return;
    }
    
    parseNameValue();
    
    if (getCurrentChar() == ':') {
        this->current_char++;
    }
    else {
        
    }
    if (getCurrentChar() == ' ') {
        this->current_char++;
    }
    else {
        
    }
    
    parseNameValueList();
}

void HeaderParser::parseName()
{
    std::string name;

    while (getCurrentChar() != ' ' && getCurrentChar() != ':') {
        name += getCurrentChar();
        this->current_char++;
    }

    this->name_values.push_back(std::make_pair(name, ""));
}

void HeaderParser::parseValue()
{
    std::string value;
    
    while (getCurrentChar() != '\r') {
        value += getCurrentChar();
        this->current_char++;
    }
    this->name_values.back().second = value;
}


