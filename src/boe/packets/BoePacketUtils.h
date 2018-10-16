/*
 * Copyright 2014-2018 Neueda Ltd.
 */
#ifndef BOE_PACKET_UTILS_H
#define BOE_PACKET_UTILS_H

#define PACKED(d) d __attribute__ ((__packed__))

namespace neueda
{
static inline bool
setString (char* s, const string& v, size_t slen)
{
    if (v.size () > slen)
        return false;
    memset (s, '\0', slen);
    memcpy (s, v.c_str (), v.length ());
    return true;
};

static inline bool
setString (char* field, unsigned char* s, size_t slen)
{
    unsigned char* stringField = (unsigned char*)field;
    for(size_t i=0;i<slen;++i)
                stringField[i] = s[i];

    return true;
};

static inline string
getString (char* v, size_t slen)
{
    string s;
    if (v[slen - 1] == '\0')
        s.assign (v);
    else
    {
        char t[1024] = {0};
        memcpy (t, v, slen);
        s.assign (t, slen);
    }

    return s;
};


static inline string
hexStr(unsigned char* data, int len)
{
    std::stringstream ss;
    ss << std::hex;
    for(int i=0;i<len;++i)
        ss <<  std::uppercase << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
};

}

#endif
