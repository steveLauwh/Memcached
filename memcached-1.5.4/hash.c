/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "memcached.h"
#include "jenkins_hash.h"
#include "murmur3_hash.h"

// 选用 hash 算法
int hash_init(enum hashfunc_type type) {
    switch(type) {
        case JENKINS_HASH:
            hash = jenkins_hash;
            settings.hash_algorithm = "jenkins"; // jenkins_hash
            break;
        case MURMUR3_HASH:
            hash = MurmurHash3_x86_32;
            settings.hash_algorithm = "murmur3"; // murmur3_hash
            break;
        default: 
            return -1;
    }
    return 0;
}
