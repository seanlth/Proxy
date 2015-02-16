//
//  HeaderParser.h
//  Proxy
//
//  Created by Seán Hargadon on 16/02/2015.
//  Copyright (c) 2015 Seán Hargadon. All rights reserved.
//

#ifndef __Proxy__HeaderParser__
#define __Proxy__HeaderParser__

#include <stdio.h>
#include <vector>
#include <string>

class HeaderParser {
private:
    char* buffer;
    int buffer_size;
    
    std::vector<std::pair<std::string, std::string>> name_values;
    int current_name_value;
    
    int current_char;
    
    char getCurrentChar();
    
    void parseNameValue();
    void parseStatus();
    void parseNameValueList();
    void parseName();
    void parseValue();
    
public:
    HeaderParser(char* buffer, int buffer_size);
    std::vector<std::pair<std::string, std::string>> parseHeader();
};



#endif /* defined(__Proxy__HeaderParser__) */
