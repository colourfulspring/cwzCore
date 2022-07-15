// extent wire protocol

#ifndef extent_protocol_h
#define extent_protocol_h

enum types { T_DIR = 1,
             T_FILE,
             T_LINK };

struct attr {
    unsigned int type;
    unsigned int atime;
    unsigned int mtime;
    unsigned int ctime;
    unsigned int size;
};

#endif
